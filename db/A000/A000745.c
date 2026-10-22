/*
 * A000745  Boustrophedon transform of squares.
 * https://oeis.org/A000745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Boustrophedon transform of b(k) = (k+1)^2: T(k,0) = b(k), T(k,j)
 *            = T(k,j-1) + T(k-1,k-j) for 1 <= j <= k, a(k) = T(k,k); T(0,0) =
 *            1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 13, u64 n <= 22, u128 n <= 37,
 *            big any n
 * Helpers:   a306_alloc0, a306_grow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a306_alloc0.h"
#include "../utils/a306_grow.h"

static seq_t *bt_res = NULL;
static seq_t *bt_prev = NULL;
static seq_t *bt_cur = NULL;
static idx_t bt_cap = 0;
static idx_t bt_done = 0;

static void bt_ensure(idx_t need) {
    if (bt_cap == 0) {
        idx_t c = need < 16 ? 16 : need;
        bt_res = a306_alloc0(c);
        bt_prev = a306_alloc0(c);
        bt_cur = a306_alloc0(c);
        bt_cap = c;
        return;
    }
    if (need > bt_cap) {
        idx_t c = bt_cap * 2;
        if (c < need) c = need;
        bt_res = a306_grow(bt_res, bt_cap, c);
        bt_prev = a306_grow(bt_prev, bt_cap, c);
        bt_cur = a306_grow(bt_cur, bt_cap, c);
        bt_cap = c;
    }
}

seq_t a(idx_t n) {
    bt_ensure(n + 1);
    if (bt_done == 0) {
        bt_prev[0] = 1;
        bt_res[0] = 1;
        bt_done = 1;
    }
    while (bt_done <= n) {
        idx_t k = bt_done;
        seq_t *t;
        idx_t j;
        bt_cur[0] = ((seq_t)k + 1) * ((seq_t)k + 1);
        for (j = 1; j <= k; j++)
            bt_cur[j] = bt_cur[j - 1] + bt_prev[k - j];
        bt_res[k] = bt_cur[k];
        t = bt_prev;
        bt_prev = bt_cur;
        bt_cur = t;
        bt_done++;
    }
    return bt_res[n];
}
