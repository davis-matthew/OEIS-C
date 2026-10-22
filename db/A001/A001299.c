/*
 * A001299  Number of ways of making change for n cents using coins of 1, 5,
 *          10, 25 cents.
 * https://oeis.org/A001299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 769, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Dynamic programming coin change: count partitions of n into {1, 5, 10, 25} */
seq_t a(idx_t n) {
    seq_t *dp = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    dp[0] = 1;
    
    const int coins[] = {1, 5, 10, 25};
    for (int c = 0; c < 4; c++) {
        idx_t coin = (idx_t)coins[c];
        for (idx_t i = coin; i <= n; i++) {
            dp[i] += dp[i - coin];
        }
    }
    
    seq_t result = dp[n];
    free(dp);
    return result;
}
