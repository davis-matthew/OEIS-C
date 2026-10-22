/*
 * A004895  Numbers that are the sum of at most 11 positive 9th powers.
 * https://oeis.org/A004895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers that are the sum of at most 11 positive 9th powers
 * Range:     u8 n <= 12, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>


seq_t a(idx_t n) {
    static uint8_t *dp = NULL;
    static idx_t dp_limit = 0;
    
    if (n < 1) return 0;
    
    if (dp == NULL) {
        dp_limit = 2000000;
        dp = (uint8_t *)malloc(dp_limit);
        
        dp[0] = 0;
        for (idx_t i = 1; i < dp_limit; i++) {
            dp[i] = 255;
        }
        
        // Process each 9th power
        for (idx_t base = 1; base <= 10; base++) {
            idx_t power = 1;
            for (int e = 0; e < 9; e++) {
                power *= base;
                if (power >= dp_limit) break;
            }
            
            if (power >= dp_limit) break;
            
            // Update DP: dp[v] = min count of 9th powers to sum to v
            for (idx_t v = power; v < dp_limit; v++) {
                if (dp[v - power] < 11) {
                    uint8_t nc = dp[v - power] + 1;
                    if (nc < dp[v]) dp[v] = nc;
                }
            }
        }
    }
    
    // Find n-th value with dp[v] <= 11
    idx_t cnt = 0;
    for (idx_t v = 0; v < dp_limit; v++) {
        if (dp[v] <= 11) {
            cnt++;
            if (cnt == n) return (seq_t)v;
        }
    }
    
    return 0;
}
