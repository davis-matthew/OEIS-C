/*
 * A004860  Numbers that are the sum of at most 9 nonzero 6th powers.
 * https://oeis.org/A004860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 2365, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Dynamic programming to enumerate numbers that are sums of at most 9 nonzero 6th powers */

seq_t a(idx_t n) {
    idx_t limit = 1000000;
    
    uint8_t *dp = (uint8_t *)malloc(limit + 1);
    memset(dp, 255, limit + 1);
    dp[0] = 0;
    
    // Process each 6th power
    for (idx_t base = 1; base <= 10; base++) {
        idx_t base2 = base * base;
        idx_t base3 = base2 * base;
        idx_t power6 = base3 * base3;
        
        if (power6 > limit) break;
        
        // Update DP: dp[i] = min count of 6th powers summing to i
        for (idx_t i = power6; i <= limit; i++) {
            if (dp[i - power6] < 9) {
                uint8_t new_val = dp[i - power6] + 1;
                if (new_val < dp[i]) {
                    dp[i] = new_val;
                }
            }
        }
    }
    
    // Find and return the n-th representable number
    idx_t count = 0;
    seq_t result = 0;
    for (idx_t i = 0; i <= limit; i++) {
        if (dp[i] <= 9) {
            count++;
            if (count == n) {
                result = (seq_t)i;
                break;
            }
        }
    }
    
    free(dp);
    return result;
}
