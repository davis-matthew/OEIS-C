/*
 * A006213  Number of down-up permutations of n+4 starting with n+1.
 * https://oeis.org/A006213
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = T(n+3, n) where T is the Seidel-Entringer triangle
 *            A008282: T(0,0) = 1, T(m,0) = 0 for m > 0, T(m,k) = T(m,k-1) +
 *            T(m-1,m-k) for 1 <= k <= m
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/grow_zero.h"

static seq_t *row_cur = NULL;   /* holds row cur_idx of the triangle */
static seq_t *row_tmp = NULL;
static idx_t row_cap = 0;
static idx_t cur_idx = 0;
static int have_row = 0;

seq_t a(idx_t n) {
    idx_t m = n + 3;
    idx_t r, k;

    if (row_cap < m + 1) {
        idx_t newcap = m + 1;
        if (newcap < 2 * row_cap) newcap = 2 * row_cap;
        row_cur = grow_zero(row_cur, row_cap, newcap);
        row_tmp = grow_zero(row_tmp, row_cap, newcap);
        row_cap = newcap;
    }

    if (!have_row || cur_idx > m) {
        row_cur[0] = 1;
        cur_idx = 0;
        have_row = 1;
    }

    while (cur_idx < m) {
        seq_t *t;
        r = cur_idx + 1;
        row_tmp[0] = 0;
        for (k = 1; k <= r; k++) {
            row_tmp[k] = row_tmp[k - 1] + row_cur[r - k];
        }
        t = row_cur;
        row_cur = row_tmp;
        row_tmp = t;
        cur_idx = r;
    }

    return row_cur[n];
}
