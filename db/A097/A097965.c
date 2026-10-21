/*
 * A097965  Number of compositions (ordered partitions) of n into n parts,
 *          allowing zeros, with distinct nonzero parts.
 * https://oeis.org/A097965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{⌊(√(8n+1)-1)/2⌋} n!/(n-k)! * p_distinct(n,k)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 27, u64 n <= 63, u128 n <= 160,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where p_distinct(n,k) = number of partitions of n into k distinct parts. */
/* p_distinct(n,k) = P(n - k(k-1)/2, k)  (partitions into exactly k parts). */

#include <stdlib.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* kmax = largest k with k(k+1)/2 ≤ n */
    idx_t kmax = (idx_t)((sqrt(8.0 * (double)n + 1.0) - 1.0) / 2.0);
    /* +1 to be safe for array bounds and to include kmax itself */
    kmax += 1;

    size_t rows = (size_t)(n + 1);
    size_t cols = (size_t)(kmax + 1);
    size_t total = rows * cols;

    seq_t *dp = (seq_t *)malloc(total * sizeof(seq_t));
    if (!dp) return 0;

    /* zero initialise */
    for (size_t i = 0; i < total; ++i) dp[i] = 0;

    /* base case: partition of 0 into 0 parts */
    dp[0] = 1;

    /* dynamic programming for partitions into exactly k parts */
    for (idx_t s = 1; s <= n; ++s) {
        size_t s_off = (size_t)s * cols;
        for (idx_t k = 1; k <= kmax && k <= s; ++k) {
            seq_t val = 0;
            /* add partition where one part is 1: P(s-1,k-1) */
            val += dp[((size_t)(s - 1)) * cols + (size_t)(k - 1)];
            /* add partitions where all parts ≥2: P(s-k,k) */
            if (s - k >= 0)
                val += dp[((size_t)(s - k)) * cols + (size_t)k];
            dp[s_off + (size_t)k] = val;
        }
    }

    seq_t ans = 0;
    seq_t fact_part = 1;   /* this holds n!/(n-k)! for current k */

    for (idx_t k = 1; k <= kmax; ++k) {
        /* n' = n - k(k-1)/2 */
        idx_t n_prime = n - (k * (k - 1)) / 2;
        if (n_prime > (idx_t)n || n_prime < (idx_t)0) {
            /* nothing to add */
        } else {
            seq_t p = dp[(size_t)n_prime * cols + (size_t)k];
            if (p != 0) {
                /* multiply factorial_part by the next term (n-k+1) */
                fact_part *= (seq_t)(n - k + 1);
                ans += p * fact_part;
            } else {
                /* still need to update factorial_part for next k */
                fact_part *= (seq_t)(n - k + 1);
            }
        }
    }

    free(dp);
    return ans;
}
