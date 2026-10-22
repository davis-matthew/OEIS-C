/*
 * A002292  Related to representation as sums of squares.
 * https://oeis.org/A002292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |[q^(2n+1)] (eta(2z)^12 + 32*eta(2z)^4*eta(8z)^8)|, the
 *            weight-6 level-8 newform; with x = q^2 and P(x) = Prod_{k>=1} (1
 *            - x^k): a(n) = |[x^n] P(x)^12 + 32 * Sum_{j>=0} [x^(n-1-4j)]
 *            P(x)^4 * [x^j] P(x)^8|
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 2231, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a2292_smul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a2292_smul.h"

seq_t a(idx_t n)
{
    long long N = (long long)n;
    long long L = N + 1;
    long long *A = (long long *)calloc((size_t)L, sizeof(long long));
    long long v, j;
    int t;
    A[0] = 1;
    for (t = 0; t < 4; t++) a2292_smul(A, L, 0); /* P^12 */
    v = A[N];
    free(A);
    if (N >= 1) {
        long long m = (N - 1) / 4;
        long long *B = (long long *)calloc((size_t)N, sizeof(long long));
        long long *C = (long long *)calloc((size_t)(m + 1), sizeof(long long));
        long long s = 0;
        B[0] = 1;
        a2292_smul(B, N, 0);
        a2292_smul(B, N, 1); /* P^4 */
        C[0] = 1;
        a2292_smul(C, m + 1, 0);
        a2292_smul(C, m + 1, 0);
        a2292_smul(C, m + 1, 1);
        a2292_smul(C, m + 1, 1); /* P^8 */
        for (j = 0; j <= m; j++) s += B[N - 1 - 4 * j] * C[j];
        v += 32 * s;
        free(B);
        free(C);
    }
    if (v < 0) v = -v;
    return (seq_t)(unsigned long long)v;
}
