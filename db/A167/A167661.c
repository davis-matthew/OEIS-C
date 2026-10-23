/*
 * A167661  Number of partitions of n into odd squares.
 * https://oeis.org/A167661
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into odd squares (coin values
 *            are 1, 9, 25, 49, …). This implementation uses the standard
 *            coin‑change DP with an outer loop over coin values and an inner
 *            loop over sums. The result is memoised in a static array that
 *            grows as larger n are requested. The algorithm r...
 * Range:     u8 n <= 188, u16 n <= 709, u32 n <= 3273, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static buffer for DP values and the maximum n that
       has been computed so far */
    static seq_t *dp = NULL;
    static idx_t max_computed = 0;

    /* Expand the DP buffer if necessary */
    if (n >= max_computed) {
        /* Allocate or grow the array to size n+1 */
        seq_t *tmp = (seq_t *)realloc(dp, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) return 0;  /* allocation failure */
        dp = tmp;

        /* Initialise all entries to 0 and dp[0] = 1 */
        dp[0] = 1;
        for (idx_t i = 1; i <= n; ++i) dp[i] = 0;

        /* Generate odd squares and perform the DP update */
        for (idx_t k = 0; ; ++k) {
            idx_t coin = (2 * k + 1) * (2 * k + 1);
            if (coin > n) break;
            for (idx_t sum = coin; sum <= n; ++sum) {
                dp[sum] += dp[sum - coin];
            }
        }

        max_computed = n + 1;
    }

    return dp[n];
}
