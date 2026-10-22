/*
 * A003332  Numbers that are the sum of 9 positive cubes.
 * https://oeis.org/A003332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   for n > 357, a(n) = 114 + n; otherwise a(n) is the n-th positive
 *            integer that is a sum of exactly 9 positive cubes
 * Range:     u8 n <= 154, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>


seq_t a(idx_t n) {
    if (n > 357) {
        return (seq_t)(114 + n);
    }
    
    static bool *is_sum = NULL;
    static bool initialized = false;
    
    if (!initialized) {
        idx_t limit = 500;
        is_sum = (bool *)calloc(limit + 1, sizeof(bool));
        
        // DP table: sums[k][s] = true iff s is a sum of exactly k positive cubes
        bool **sums = (bool **)malloc(10 * sizeof(bool *));
        for (int k = 0; k <= 9; k++) {
            sums[k] = (bool *)calloc(limit + 1, sizeof(bool));
        }
        
        // Base case: sums of 1 cube
        for (idx_t i = 1; i * i * i <= limit; i++) {
            sums[1][i * i * i] = true;
        }
        
        // Compute sums of k cubes for k = 2 to 9
        for (int k = 2; k <= 9; k++) {
            for (idx_t s = 1; s <= limit; s++) {
                if (sums[k-1][s]) {
                    for (idx_t i = 1; s + i * i * i <= limit; i++) {
                        sums[k][s + i * i * i] = true;
                    }
                }
            }
        }
        
        // Copy sums of exactly 9 cubes to is_sum
        for (idx_t i = 1; i <= limit; i++) {
            is_sum[i] = sums[9][i];
        }
        
        // Free DP table
        for (int k = 0; k <= 9; k++) {
            free(sums[k]);
        }
        free(sums);
        
        initialized = true;
    }
    
    // Find the n-th number that is a sum of 9 cubes
    idx_t count = 0;
    idx_t limit = 500;
    for (idx_t i = 1; i <= limit; i++) {
        if (is_sum[i]) {
            count++;
            if (count == n) {
                return (seq_t)i;
            }
        }
    }
    
    return 0;
}
