/*
 * A000752  Boustrophedon transform of powers of 2.
 * https://oeis.org/A000752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Boustrophedon transform of 2^k: T(k,0) = 2^k, T(k,j) = T(k,j-1)
 *            + T(k-1,k-j) for 1 <= j <= k; a(n) = T(n,n), a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 13, u64 n <= 22, u128 n <= 37,
 *            big any n
 * Helpers:   a306_grow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a306_grow.h"

static seq_t *bt_row = NULL;   /* row 'bt_done' of the triangle */
static seq_t *bt_tmp = NULL;
static seq_t *bt_res = NULL;   /* bt_res[i] = a(i) for i <= bt_done */
static idx_t bt_cap = 0;
static idx_t bt_done = 0;
static int bt_init = 0;

static void bt_reserve(idx_t need) {
    idx_t nc;
    if (need <= bt_cap) return;
    nc = bt_cap ? bt_cap : 16;
    while (nc < need) nc *= 2;
    bt_row = a306_grow(bt_row, bt_cap, nc);
    bt_tmp = a306_grow(bt_tmp, bt_cap, nc);
    bt_res = a306_grow(bt_res, bt_cap, nc);
    bt_cap = nc;
}

seq_t a(idx_t n) {
    idx_t r, k;
    if (!bt_init) {
        bt_reserve(n + 1 > 16 ? n + 1 : 16);
        bt_row[0] = 1;
        bt_res[0] = 1;
        bt_done = 0;
        bt_init = 1;
    }
    if (n <= bt_done) return bt_res[n];
    bt_reserve(n + 1);
    for (r = bt_done + 1; r <= n; r++) {
        seq_t *t;
        bt_tmp[0] = bt_row[0] * 2;
        for (k = 1; k <= r; k++)
            bt_tmp[k] = bt_tmp[k - 1] + bt_row[r - k];
        t = bt_row;
        bt_row = bt_tmp;
        bt_tmp = t;
        bt_res[r] = bt_row[r];
    }
    bt_done = n;
    return bt_res[n];
}
