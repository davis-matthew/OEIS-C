/*
 * A004828  Numbers that are the sum of at most 6 positive cubes.
 * https://oeis.org/A004828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers that can be expressed as the sum of at most 6 positive
 *            cubes
 * Range:     u8 n <= 202, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n) {
    // Estimate upper bound: assume ~85% of numbers are in sequence
    seq_t limit = (seq_t)((long long)n * 100 / 85 + 10);
    
    while (1) {
        // Allocate and initialize DP array
        seq_t *dp = (seq_t *)malloc((size_t)((long long)limit + 1) * sizeof(seq_t));
        for (seq_t i = 0; i <= limit; i++) {
            dp[(size_t)(long long)i] = 7; // 7 = more than 6 cubes
        }
        dp[0] = 0;
        
        // Find largest cube root <= limit
        seq_t max_cube = 1;
        while (max_cube * max_cube * max_cube <= limit) {
            max_cube++;
        }
        max_cube--;
        
        // DP: for each cube k^3, update reachable numbers
        for (seq_t k = 1; k <= max_cube; k++) {
            seq_t k3 = k * k * k;
            if (k3 > limit) break;
            
            for (seq_t val = k3; val <= limit; val++) {
                seq_t prev = dp[(size_t)(long long)(val - k3)];
                if (prev <= 6) {
                    seq_t new_count = prev + 1;
                    if (new_count < dp[(size_t)(long long)val]) {
                        dp[(size_t)(long long)val] = new_count;
                    }
                }
            }
        }
        
        // Count and find the n-th term
        idx_t count = 0;
        for (seq_t m = 0; m <= limit; m++) {
            if (dp[(size_t)(long long)m] <= 6) {
                count++;
                if (count == n) {
                    seq_t result = m;
                    free(dp);
                    return result;
                }
            }
        }
        
        free(dp);
        limit = limit * 2;
    }
}
