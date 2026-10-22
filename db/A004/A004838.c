/*
 * A004838  Numbers that are the sum of at most 9 nonzero 4th powers.
 * https://oeis.org/A004838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers that are the sum of at most 9 nonzero 4th powers
 * Range:     u8 n <= 122, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    
    if (cache == NULL) {
        // Determine the limit for DP based on seq_t's range
        idx_t limit;
        if (sizeof(seq_t) == 1) {
            limit = 255;
        } else if (sizeof(seq_t) == 2) {
            limit = 65535;
        } else {
            limit = 1000000;
        }
        
        // Allocate DP array
        uint8_t *dp = (uint8_t *)malloc(limit + 1);
        if (dp == NULL) return 0;
        
        // Initialize: dp[i] = minimum fourth powers needed to sum to i
        memset(dp, 255, limit + 1);
        dp[0] = 0;
        
        // Precompute fourth powers
        seq_t powers4[100];
        idx_t num_powers = 0;
        for (idx_t p = 1; num_powers < 100; p++) {
            idx_t p4 = p * p * p * p;
            if (p4 > limit) break;
            powers4[num_powers++] = (seq_t)p4;
        }
        
        // DP: for each reachable value, try adding each fourth power
        for (idx_t i = 0; i <= limit; i++) {
            if (dp[i] < 9) {
                for (idx_t j = 0; j < num_powers; j++) {
                    idx_t new_i = i + (idx_t)powers4[j];
                    if (new_i > limit) break;
                    if (dp[new_i] > dp[i] + 1) {
                        dp[new_i] = dp[i] + 1;
                    }
                }
            }
        }
        
        // Count valid numbers (those expressible with at most 9 fourth powers)
        idx_t count = 0;
        for (idx_t i = 0; i <= limit; i++) {
            if (dp[i] <= 9) count++;
        }
        
        // Store in cache
        cache = (seq_t *)malloc(count * sizeof(seq_t));
        if (cache == NULL) {
            free(dp);
            return 0;
        }
        cache_len = count;
        
        count = 0;
        for (idx_t i = 0; i <= limit; i++) {
            if (dp[i] <= 9) {
                cache[count++] = (seq_t)i;
            }
        }
        
        free(dp);
    }
    
    if (n < 1 || n > cache_len) {
        return 0;
    }
    
    return cache[n - 1];
}
