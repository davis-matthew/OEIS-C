/*
 * A003581  Dowling numbers: e.g.f. exp(x + (exp(b*x)-1)/b) with b=9.
 * https://oeis.org/A003581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   E.g.f. A(x) = exp(x + (exp(9x)-1)/9), so A'(x) = A(x)*(1 +
 *            exp(9x)), giving a(m) = Sum_{j=0..m-1} binomial(m-1,j) * c(j) *
 *            a(m-1-j), with c(0) = 2, c(j) = 9^j for j >= 1, a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   A_grow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/A_grow.h"

static seq_t *a3581_B = NULL;    /* a(0..cnt-1) */
static seq_t *a3581_row = NULL;  /* binomial(cnt-1, j), j = 0..cnt-1 */
static seq_t *a3581_p9 = NULL;   /* 9^j, j = 0..cnt-1 */
static idx_t a3581_cnt = 0;
static idx_t a3581_cap = 0;

seq_t a(idx_t n) {
    if (n + 1 > a3581_cap) {
        idx_t newcap = a3581_cap * 2;
        if (newcap < n + 1) newcap = n + 1;
        if (newcap < 16) newcap = 16;
        a3581_B = A_grow(a3581_B, a3581_cap, newcap);
        a3581_row = A_grow(a3581_row, a3581_cap, newcap);
        a3581_p9 = A_grow(a3581_p9, a3581_cap, newcap);
        a3581_cap = newcap;
    }
    if (a3581_cnt == 0) {
        a3581_B[0] = 1;
        a3581_row[0] = 1;
        a3581_p9[0] = 1;
        a3581_cnt = 1;
    }
    while (a3581_cnt <= n) {
        idx_t m = a3581_cnt;
        seq_t s = 0;
        for (idx_t j = 0; j < m; j++) {
            seq_t c = (j == 0) ? (seq_t)2 : a3581_p9[j];
            s += a3581_row[j] * c * a3581_B[m - 1 - j];
        }
        a3581_B[m] = s;
        /* advance row to binomial(m, j) */
        a3581_row[m] = 1;
        for (idx_t j = m - 1; j >= 1; j--) {
            a3581_row[j] += a3581_row[j - 1];
        }
        a3581_p9[m] = a3581_p9[m - 1] * 9;
        a3581_cnt = m + 1;
    }
    return a3581_B[n];
}
