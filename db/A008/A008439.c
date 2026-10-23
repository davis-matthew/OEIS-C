/*
 * A008439  Expansion of Jacobi theta constant theta_2^5 /32.
 * https://oeis.org/A008439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = (5/n) * Σ_{k=1..n} A002129(k) * a(n-k)
 *            (A002129(k) = 1 if k is a triangular number, 0 otherwise)
 *            Computed by 5 successive convolutions of the
 *            triangular‑indicator
 * Range:     u8 n <= 21, u16 n <= 852, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* trivial case */
    if (n == 0)
        return 1;

    size_t sz = (size_t)n + 1;
    seq_t *dp   = (seq_t*)calloc(sz, sizeof(seq_t));
    seq_t *next = (seq_t*)calloc(sz, sizeof(seq_t));
    if (!dp || !next) {
        free(dp); free(next);
        return 0; /* allocation failure – unlikely */
    }

    dp[0] = 1;  /* empty sum */

    /* maximum index k with k(k+1)/2 <= n */
    idx_t max_k = 0;
    while ((max_k * (max_k + 1) / 2) <= n)
        ++max_k;

    /* perform five successive convolutions with triangular numbers */
    for (idx_t iter = 0; iter < 5; ++iter) {
        /* clear the next array */
        for (size_t i = 0; i < sz; ++i)
            next[i] = 0;

        /* add contributions of each triangular number */
        for (idx_t k = 0; k < max_k; ++k) {
            idx_t tri = k * (k + 1) / 2;
            for (idx_t j = tri; j <= n; ++j) {
                next[j] = next[j] + dp[j - tri];
            }
        }

        /* swap dp and next for the next iteration */
        seq_t *tmp = dp;
        dp = next;
        next = tmp;
    }

    seq_t result = dp[n];

    free(dp);
    free(next);
    return result;
}
