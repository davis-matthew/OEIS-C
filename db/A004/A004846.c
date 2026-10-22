/*
 * A004846  Numbers that are the sum of at most 6 positive 5th powers.
 * https://oeis.org/A004846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers that are the sum of at most 6 positive 5th powers
 * Range:     u8 n <= 14, u16 n <= 1045, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


seq_t a(idx_t n) {
    idx_t limit = 1000000;
    
    bool *can_make = (bool *)calloc(limit + 1, sizeof(bool));
    if (!can_make) return 0;
    
    // Collect fifth powers
    seq_t powers[100];
    int num_powers = 0;
    
    for (seq_t i = 1; i <= 100; i++) {
        seq_t p = i * i * i * i * i;
        if (p > (seq_t)limit) break;
        powers[num_powers++] = p;
    }
    
    can_make[0] = true;
    
    // Build up sums using at most 6 fifth powers
    for (int depth = 0; depth < 6; depth++) {
        bool *prev = (bool *)malloc((limit + 1) * sizeof(bool));
        memcpy(prev, can_make, (limit + 1) * sizeof(bool));
        
        for (idx_t i = 0; i <= limit; i++) {
            if (prev[i]) {
                for (int p = 0; p < num_powers; p++) {
                    seq_t power = powers[p];
                    if (power <= (seq_t)(limit - i)) {
                        can_make[i + (idx_t)power] = true;
                    }
                }
            }
        }
        
        free(prev);
    }
    
    // Find the nth value
    idx_t count = 0;
    for (idx_t i = 0; i <= limit; i++) {
        if (can_make[i]) {
            count++;
            if (count == n) {
                seq_t result = (seq_t)i;
                free(can_make);
                return result;
            }
        }
    }
    
    free(can_make);
    return 0;
}
