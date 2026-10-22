/*
 * A001301  Number of ways of making change for n cents using coins of 1, 2,
 *          5, 10, 25 cents.
 * https://oeis.org/A001301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   number of partitions of n into parts {1, 2, 5, 10, 25}
 * Range:     u8 n <= 42, u16 n <= 229, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    seq_t *dp = (seq_t *)malloc(((size_t)n + 1) * sizeof(seq_t));
    if (!dp) return 0;
    
    memset(dp, 0, ((size_t)n + 1) * sizeof(seq_t));
    dp[0] = 1;
    
    static const int coins[] = {1, 2, 5, 10, 25};
    for (int c = 0; c < 5; c++) {
        idx_t coin = (idx_t)coins[c];
        for (idx_t i = coin; i <= n; i++) {
            dp[i] = dp[i] + dp[i - coin];
        }
    }
    
    seq_t result = dp[n];
    free(dp);
    return result;
}
