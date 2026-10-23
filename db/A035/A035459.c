/*
 * A035459  Number of partitions of n into parts 8k+3 or 8k+4.
 * https://oeis.org/A035459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   number of partitions of n into parts 8k+3 or 8k+4 Computed by
 *            dynamic programming over the allowed part sizes. Time
 *            complexity: O(n^2 / 8), memory O(n)
 * Range:     u8 n <= 66, u16 n <= 169, u32 n <= 495, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. For n < 1 we return 0. */
    if (n == 0) return 0;

    /* Allocate DP array of size n+1. */
    seq_t *dp = (seq_t*)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp) return 0;                     /* allocation failure */
    dp[0] = 1;
    for (idx_t i = 1; i <= n; ++i) dp[i] = 0;

    /* Add parts of the form 8k+3. */
    for (idx_t k = 0;; ++k) {
        idx_t part = 8 * k + 3;
        if (part > n) break;
        for (idx_t i = part; i <= n; ++i) {
            dp[i] = dp[i] + dp[i - part];
        }
    }

    /* Add parts of the form 8k+4. */
    for (idx_t k = 0;; ++k) {
        idx_t part = 8 * k + 4;
        if (part > n) break;
        for (idx_t i = part; i <= n; ++i) {
            dp[i] = dp[i] + dp[i - part];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
