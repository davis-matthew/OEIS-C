/*
 * A004840  Numbers that are the sum of at most 11 nonzero 4th powers.
 * https://oeis.org/A004840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Sieve DP to find numbers expressible as sums of at most 11
 *            nonzero 4th powers
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>
#include <stdlib.h>

static seq_t *cached_sequence = NULL;
static idx_t cached_len = 0;

seq_t a(idx_t n) {
    if (cached_sequence == NULL) {
        const idx_t limit = 200000;
        uint8_t *min_parts = (uint8_t *)malloc((size_t)limit * sizeof(uint8_t));
        memset(min_parts, 255, (size_t)limit * sizeof(uint8_t));
        min_parts[0] = 0;
        
        for (idx_t k = 1; k * k * k * k < limit; k++) {
            seq_t fourth = (seq_t)(k * k * k * k);
            idx_t fourth_idx = (idx_t)fourth;
            
            for (idx_t i = fourth_idx; i < limit; i++) {
                idx_t prev = i - fourth_idx;
                if (min_parts[prev] < 11) {
                    uint8_t new_parts = min_parts[prev] + 1;
                    if (min_parts[i] > new_parts) {
                        min_parts[i] = new_parts;
                    }
                }
            }
        }
        
        cached_len = 0;
        for (idx_t i = 0; i < limit; i++) {
            if (min_parts[i] <= 11) cached_len++;
        }
        
        cached_sequence = (seq_t *)malloc((size_t)cached_len * sizeof(seq_t));
        idx_t idx = 0;
        for (idx_t i = 0; i < limit; i++) {
            if (min_parts[i] <= 11) {
                cached_sequence[idx++] = (seq_t)i;
            }
        }
        
        free(min_parts);
    }
    
    return cached_sequence[n - 1];
}
