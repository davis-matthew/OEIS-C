/*
 * A001314  Number of (unordered) ways of making change for n cents using
 *          coins of 2, 5 (two kinds), 10, 20, 50 cents.
 * https://oeis.org/A001314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Number of ways to partition n using coins {2, 5, 5, 10, 20, 50}
 * Range:     u8 n <= 64, u16 n <= 284, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    int coins[] = {2, 5, 5, 10, 20, 50};
    int num_coins = 6;
    
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    
    for (idx_t i = 0; i <= n; i++) dp[i] = 0;
    dp[0] = 1;
    
    for (int c = 0; c < num_coins; c++) {
        int coin = coins[c];
        for (idx_t i = (idx_t)coin; i <= n; i++) {
            dp[i] += dp[i - coin];
        }
    }
    
    seq_t result = dp[n];
    free(dp);
    return result;
}
