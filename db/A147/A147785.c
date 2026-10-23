/*
 * A147785  Number of partitions of n into parts divisible by 3 or 5.
 * https://oeis.org/A147785
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions to n = sum_{i} p_i, p_i ∈
 *            {3,5,6,9,10,…}
 * Range:     u8 n <= 44, u16 n <= 107, u32 n <= 297, u64 n <= 933,
 *            u128 n <= 3199, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A147785: Number of partitions of n into parts divisible by 3 or 5 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache holding all computed values up to the current limit.
       dp_len is one greater than the largest index currently stored. */
    static seq_t *dp = NULL;
    static idx_t dp_len = 0;

    /* If the cache is too small, grow it to size n+1 and recompute all values. */
    if (n + 1 > dp_len) {
        /* Allocate or reallocate */
        seq_t *new_dp = (seq_t *)realloc(dp, sizeof(seq_t) * (n + 1));
        if (!new_dp && (n + 1) > 0) {
            /* Allocation failed – return 0 as a graceful failure. */
            return (seq_t)0;
        }
        dp = new_dp;
        dp_len = n + 1;

        /* Recompute the partition counts from scratch. */
        for (idx_t i = 0; i < dp_len; ++i) {
            dp[i] = 0;
        }
        dp[0] = 1;  /* Base case: exactly one way to partition 0 */

        /* Coin‑change dynamic programming using parts divisible by 3 or 5. */
        for (idx_t coin = 3; coin < dp_len; ++coin) {
            if ((coin % 3U) == 0U || (coin % 5U) == 0U) {
                for (idx_t j = coin; j < dp_len; ++j) {
                    dp[j] += dp[j - coin];
                }
            }
        }
    }

    /* All requested values are now available. */
    return dp[n];
}
