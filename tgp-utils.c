/*
 This file is part of telegram-purple
 
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02111-1301  USA
 
 Copyright Matthias Jentsch 2014
 */

#include "tgp-utils.h"
#include <purple.h>

connection_data *get_conn_from_buddy (PurpleBuddy *buddy) {
  connection_data *c = purple_connection_get_protocol_data (
                            purple_account_get_connection (purple_buddy_get_account (buddy)));
  return c;
}

const char *format_time (time_t date) {
  struct tm *datetime = localtime(&date);
  return purple_utf8_strftime ("%d.%m.%Y %H:%M", datetime);
}

char *format_img_full (int imgstore) {
  const char *br = "<br>";
  
  // <br>'s look ugly in Adium, but no <br> will look ugly in Pidgin
#ifdef __ADIUM_
  br = "";
#endif
  return g_strdup_printf ("%s<img id=\"%u\">", br, imgstore);
}

int str_not_empty (const char *string) {
  return string && string[0] != '\0';
}

int our_msg (struct tgl_state *TLS, struct tgl_message *M) {
  return (M->flags & FLAG_SESSION_OUTBOUND) != 0;
}

int out_msg (struct tgl_state *TLS, struct tgl_message *M) {
  return M->out;
}

tgl_peer_t *find_peer_by_name (struct tgl_state *TLS, const char *who) {
  tgl_peer_t *peer = tgl_peer_get (TLS, TGL_MK_USER(atoi (who)));
  if (peer) { return peer; }
  peer = tgl_peer_get (TLS, TGL_MK_CHAT(atoi(who)));
  if (peer) { return peer; }
  peer = tgl_peer_get (TLS, TGL_MK_ENCR_CHAT(atoi(who)));
  if (peer) { return peer; }
  return NULL;
}

tgl_peer_t *tgp_encr_chat_get_partner (struct tgl_state *TLS, struct tgl_secret_chat *chat) {
  return tgl_peer_get (TLS, TGL_MK_USER(chat->admin_id == TLS->our_id ? chat->user_id : chat->admin_id));
}

char *tgp_g_format_size (gint64 size) {
  char *sizes[] = {
    "B",
    "KB",
    "MB",
    "GB",
    "PB"
  };
  int base = 0;
  double s = (double) size;
  while (s > 1024 && base < 4) {
    s /= 1024;
    ++ base;
  }
  return g_strdup_printf ("%.1f %s, ", s, sizes[base]);
}

void tgp_g_queue_free_full (GQueue *queue, GDestroyNotify free_func) {
  void *entry;
  
  while ((entry = g_queue_pop_head(queue))) {
    free_func (entry);
  }
  g_queue_free (queue);
}
