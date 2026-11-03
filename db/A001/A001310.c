/*
 * A001310  Number of ways of making change for n cents using coins of 1, 2,
 *          4, 10, 20, 40, 100 cents.
 * https://oeis.org/A001310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   number of partitions of n into parts from {1, 2, 4, 10, 20, 40,
 *            100} Using dynamic programming: dp[i] counts ways to make amount
 *            i
 * Range:     u8 n <= 39, u16 n <= 179, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 75 terms through n = 891 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"


seq_t a(idx_t n) {
    if (n == 0) return 1;
    
    seq_t *dp = mem_new(seq_t, (n + 1));
    if (!dp) return 0;
    
    for (idx_t i = 0; i <= n; i++) {
        dp[i] = 0;
    }
    dp[0] = 1;
    
    static const int coins[] = {1, 2, 4, 10, 20, 40, 100};
    
    for (int c = 0; c < 7; c++) {
        idx_t coin = (idx_t)coins[c];
        for (idx_t i = coin; i <= n; i++) {
            dp[i] += dp[i - coin];
        }
    }
    
    seq_t result = dp[n];
    mem_free(dp);
    return result;
}
