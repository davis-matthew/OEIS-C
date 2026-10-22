/*
 * A000405  Number of 6-level labeled rooted trees with n leaves.
 * https://oeis.org/A000405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   E.g.f. exp(exp(exp(exp(exp(exp(x)-1)-1)-1)-1)-1). Set L1(x) =
 *            exp(x) and Lj(x) = exp(L(j-1)(x) - 1). Since Lj' = L(j-1)' * Lj,
 *            the coefficients satisfy Lj[m] = Sum_{k=0..m-1} binomial(m-1,k)
 *            * L(j-1)[k+1] * Lj[m-1-k], with Lj[0] = 1 and L1[m] = 1. Then
 *            a(n) = L6[n]
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#define A405_MAXN 16384

static seq_t a405_L[7][A405_MAXN];
static seq_t a405_B[A405_MAXN];
static idx_t a405_done = 0;
static idx_t a405_brow = 0;

seq_t a(idx_t n) {
    idx_t m, k;
    int j;
    if (n >= A405_MAXN) return 0;
    if (a405_done == 0) {
        for (j = 1; j <= 6; j++) a405_L[j][0] = 1;
        a405_B[0] = 1;
        a405_brow = 0;
        a405_done = 1;
    }
    for (m = a405_done; m <= n; m++) {
        idx_t r = m - 1;
        while (a405_brow < r) {
            idx_t nr = a405_brow + 1;
            a405_B[nr] = 0;
            for (k = nr; k >= 1; k--) a405_B[k] += a405_B[k - 1];
            a405_brow = nr;
        }
        a405_L[1][m] = 1;
        for (j = 2; j <= 6; j++) {
            seq_t sum = 0;
            for (k = 0; k < m; k++) {
                sum += a405_B[k] * a405_L[j - 1][k + 1] * a405_L[j][m - 1 - k];
            }
            a405_L[j][m] = sum;
        }
        a405_done = m + 1;
    }
    return a405_L[6][n];
}
