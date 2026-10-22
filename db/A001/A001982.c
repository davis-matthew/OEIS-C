/*
 * A001982  Number of partitions of 4n-1 into n nonnegative integers each no
 *          greater than 8.
 * https://oeis.org/A001982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [q^(4n-1)] of the Gaussian binomial [n+8, 8]_q =
 *            [q^(4n-1)] Product_{k=1..8} (1-q^(n+k))/(1-q^k), a(0) = 0. Built
 *            one k at a time; each intermediate [n+k, k]_q has nonnegative
 *            coefficients, so subtractions never go below zero
 * Range:     u8 n <= 6, u16 n <= 20, u32 n <= 115, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;
    long long N = (long long)(4 * n - 1);
    long long L = N + 1;
    seq_t *g = (seq_t *)malloc((size_t)L * sizeof(seq_t));
    long long j;
    for (j = 0; j < L; j++) g[j] = 0;
    g[0] = 1;
    for (long long k = 1; k <= 8; k++) {
        /* divide by (1 - q^k) */
        for (j = k; j <= N; j++) g[j] += g[j - k];
        /* multiply by (1 - q^(n+k)) */
        long long m = (long long)n + k;
        for (j = N; j >= m; j--) g[j] -= g[j - m];
    }
    seq_t r = g[N];
    free(g);
    return r;
}
