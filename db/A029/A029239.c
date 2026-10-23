/*
 * A029239  Expansion of 1/((1-x^2)*(1-x^8)*(1-x^10)*(1-x^11)).
 * https://oeis.org/A029239
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   count of partitions of n into parts 2, 8, 10, 11 using DP
 * Range:     u8 n <= 119, u16 n <= 863, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* dp[i] = number of ways to write i as a sum of 2,8,10,11 (order irrelevant) */
    seq_t *dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (dp == NULL) {
        /* out of memory – unlikely for n ≤ 10000 */
        return (seq_t)0;
    }

    for (idx_t i = 0; i <= n; ++i) {
        dp[i] = (seq_t)0;
    }
    dp[0] = (seq_t)1;

    const idx_t parts[] = { 2, 8, 10, 11 };
    const size_t part_cnt = sizeof(parts) / sizeof(parts[0]);

    for (size_t pi = 0; pi < part_cnt; ++pi) {
        idx_t coin = parts[pi];
        for (idx_t i = coin; i <= n; ++i) {
            dp[i] += dp[i - coin];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
