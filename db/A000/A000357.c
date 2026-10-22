/*
 * A000357  Number of 5-level labeled rooted trees with n leaves.
 * https://oeis.org/A000357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   E.g.f. exp(exp(exp(exp(exp(x)-1)-1)-1)-1). Each level G =
 *            exp(F-1) satisfies g(0) = 1, g(m) = Sum_{j=1..m}
 *            binomial(m-1,j-1)*f(j)*g(m-j); start from f = exp(x) (all ones),
 *            apply 4 times, a(n) = coefficient n of the result times n!
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#define A357_CAP 10002

static seq_t a357_F[5][A357_CAP];
static seq_t a357_R[A357_CAP];
static idx_t a357_done = 0;
static int a357_init = 0;

seq_t a(idx_t n) {
    idx_t m, j;
    int L;
    if (!a357_init) {
        for (L = 0; L < 5; L++) a357_F[L][0] = 1;
        a357_R[0] = 1;           /* row 0 of Pascal's triangle */
        a357_done = 0;
        a357_init = 1;
    }
    if (n >= A357_CAP) n = A357_CAP - 1;
    for (m = a357_done + 1; m <= n; m++) {
        /* a357_R holds binomial(m-1, .) */
        a357_F[0][m] = 1;
        for (L = 1; L < 5; L++) {
            seq_t s = 0;
            for (j = 1; j <= m; j++) {
                s += a357_R[j - 1] * a357_F[L - 1][j] * a357_F[L][m - j];
            }
            a357_F[L][m] = s;
        }
        /* advance Pascal row to binomial(m, .) */
        a357_R[m] = 1;
        for (j = m - 1; j >= 1; j--) {
            a357_R[j] += a357_R[j - 1];
        }
        a357_done = m;
    }
    return a357_F[4][n];
}
