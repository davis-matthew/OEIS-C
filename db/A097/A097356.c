/*
 * A097356  Number of partitions of n into parts not greater than sqrt(n).
 * https://oeis.org/A097356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n with each part ≤ floor(sqrt(n))
 * Range:     u8 n <= 24, u16 n <= 63, u32 n <= 176, u64 n <= 575,
 *            u128 n <= 1976, big any n
 * Helpers:   floor_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: dynamic programming over all allowed part sizes (1 … floor(sqrt(n))) */

#include <stdlib.h>
#include "../utils/floor_isqrt.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 by convention */
    if (n == (idx_t)0) return (seq_t)1;

    /* limit = floor(sqrt(n))  */
    idx_t limit = floor_isqrt(n);

    /* Allocate dp[0..n] */
    seq_t *dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp) return (seq_t)0;          /* allocation failure -> return 0 */

    dp[0] = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= n; ++i) dp[i] = (seq_t)0;

    /* 1‑dimensional DP: for each part size k, add its contribution */
    for (idx_t k = (idx_t)1; k <= limit; ++k) {
        for (idx_t i = k; i <= n; ++i) {
            dp[i] = dp[i] + dp[i - k];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
