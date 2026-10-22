/*
 * A004700  Expansion of e.g.f. 1/(3 - exp(x) - exp(2*x)).
 * https://oeis.org/A004700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Sum_{k=1..n} binomial(n,k) * (2^k + 1) * a(n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/grow_zero.h"

static seq_t *A_memo = NULL;   /* a(0..cnt-1) */
static seq_t *P_row = NULL;    /* binomial(cnt-1, 0..cnt-1) */
static seq_t *T_fac = NULL;    /* 2^k + 1 for k = 0..cnt-1 */
static seq_t pw2 = 1;          /* 2^cnt */
static idx_t cnt = 0, cap = 0;

seq_t a(idx_t n) {
    if (n < cnt) return A_memo[n];

    if (n + 1 > cap) {
        idx_t nc = cap * 2;
        if (nc < n + 1) nc = n + 1;
        if (nc < 64) nc = 64;
        A_memo = grow_zero(A_memo, cap, nc);
        P_row = grow_zero(P_row, cap, nc);
        T_fac = grow_zero(T_fac, cap, nc);
        cap = nc;
    }

    for (idx_t m = cnt; m <= n; m++) {
        /* advance Pascal row to row m */
        P_row[m] = 1;
        for (idx_t j = m - 1; j >= 1 && m >= 2; j--) P_row[j] += P_row[j - 1];
        P_row[0] = 1;

        T_fac[m] = pw2 + 1;
        pw2 *= 2;

        if (m == 0) {
            A_memo[0] = 1;
        } else {
            seq_t s = 0;
            for (idx_t k = 1; k <= m; k++)
                s += P_row[k] * T_fac[k] * A_memo[m - k];
            A_memo[m] = s;
        }
    }
    cnt = n + 1;
    return A_memo[n];
}
