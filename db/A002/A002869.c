/*
 * A002869  Largest number in n-th row of triangle A019538.
 * https://oeis.org/A002869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = max_{1<=k<=n} T(n,k), where T(n,k) = k!*S2(n,k) satisfies
 *            T(n,k) = k*(T(n-1,k) + T(n-1,k-1)), T(0,0) = 1, T(n,0) = 0 for n
 *            >= 1; a(0) = 1
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/grow_zero.h"

static seq_t *a002869_row = NULL;  /* row[k] = T(m,k) for the last computed row m */
static seq_t *a002869_mx = NULL;   /* mx[m] = a(m) */
static idx_t a002869_cnt = 0;      /* number of maxima computed */
static idx_t a002869_cap = 0;

seq_t a(idx_t n) {
    if (n < a002869_cnt) return a002869_mx[n];

    if (n + 1 > a002869_cap) {
        idx_t newcap = a002869_cap * 2;
        if (newcap < n + 1) newcap = n + 1;
        if (newcap < 16) newcap = 16;
        a002869_row = grow_zero(a002869_row, a002869_cap, newcap);
        a002869_mx = grow_zero(a002869_mx, a002869_cap, newcap);
        a002869_cap = newcap;
    }

    if (a002869_cnt == 0) {
        a002869_row[0] = 1;
        a002869_mx[0] = 1;
        a002869_cnt = 1;
    }

    while (a002869_cnt <= n) {
        idx_t m = a002869_cnt;
        seq_t best = 0;
        for (idx_t k = m; k >= 1; k--) {
            a002869_row[k] = (seq_t)k * (a002869_row[k] + a002869_row[k - 1]);
            if (a002869_row[k] > best) best = a002869_row[k];
        }
        a002869_row[0] = 0;
        a002869_mx[m] = best;
        a002869_cnt++;
    }

    return a002869_mx[n];
}
