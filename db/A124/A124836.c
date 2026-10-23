/*
 * A124836  Central terms of even-indexed rows in triangle A124834.
 * https://oeis.org/A124836
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k=0}^n 1/(1 - binomial(n,k)·x)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute A124836: central terms of even-indexed rows in triangle A124834 */
seq_t a(idx_t n)
{
    /* Trivial case */
    if (n == 0)
        return (seq_t)1;

    /* Allocate two arrays of size n+1 for dynamic programming */
    size_t size = (size_t)n + 1;
    seq_t *dp    = (seq_t *)malloc(size * sizeof(seq_t));
    seq_t *newdp = (seq_t *)malloc(size * sizeof(seq_t));
    if (!dp || !newdp) {
        /* In practice this should never happen for the test ranges */
        if (dp) free(dp);
        if (newdp) free(newdp);
        return (seq_t)0;
    }

    /* Initialise dp[0] = 1, rest 0 */
    for (size_t i = 0; i < size; ++i)
        dp[i] = 0;
    dp[0] = 1;

    seq_t bin = 1;          /* binomial(n,0) */

    for (idx_t k = 0; k <= n; ++k) {
        if (k > 0) {
            /* Update bin = binomial(n,k) using recurrence */
            seq_t num   = (seq_t)(n - k + 1);
            seq_t denom = (seq_t)k;
            bin = bin * num / denom;
        }

        /* Compute newdp from dp and current binomial coefficient */
        newdp[0] = dp[0];
        for (size_t s = 1; s < size; ++s) {
            newdp[s] = dp[s] + bin * newdp[s - 1];
        }

        /* Swap pointers for next iteration */
        seq_t *tmp = dp;
        dp = newdp;
        newdp = tmp;
    }

    seq_t result = dp[n];
    free(dp);
    free(newdp);
    return result;
}
