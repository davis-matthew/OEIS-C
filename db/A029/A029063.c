/*
 * A029063  Expansion of 1/((1-x)*(1-x^4)*(1-x^5)*(1-x^6)).
 * https://oeis.org/A029063
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 353, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A029063: Number of partitions of n into parts 1, 4, 5 and 6.
   Computed by a simple coin‑change dynamic programming: for each coin c
   and each n ≥ c, dp[n] += dp[n - c].
*/

#include <stdlib.h>

static seq_t *dp = NULL;          /* cache of computed values */
static idx_t max_cached = 0;      /* largest n that has been computed */

seq_t a(idx_t n)
{
    /* Grow the cache if necessary and recompute all values up to n. */
    if (!dp || n > max_cached) {
        /* Reallocate to size n+1.  Use realloc to preserve existing data
           in case we grow from a smaller size. */
        seq_t *tmp = (seq_t *)realloc(dp, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – fall back to previous cache. */
            return (dp && max_cached <= n) ? dp[n] : (seq_t)0;
        }
        dp = tmp;

        /* Reset the array and set dp[0] = 1 (empty partition). */
        dp[0] = (seq_t)1;
        for (idx_t i = 1; i <= n; ++i)
            dp[i] = (seq_t)0;

        /* Coins that may be used.  The constant array is small, so
           its size is determined at compile time. */
        static const idx_t coins[4] = {1, 4, 5, 6};

        /* Standard coin change DP: for each coin, update all sums. */
        for (size_t ci = 0; ci < sizeof(coins)/sizeof(coins[0]); ++ci) {
            idx_t coin = coins[ci];
            for (idx_t i = coin; i <= n; ++i) {
                dp[i] += dp[i - coin];
            }
        }
        max_cached = n;
    }

    return dp[n];
}
