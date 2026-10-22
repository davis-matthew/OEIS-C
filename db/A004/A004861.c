/*
 * A004861  Numbers that are the sum of at most 10 nonzero 6th powers.
 * https://oeis.org/A004861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A004861 - numbers that are sum of at most 10 nonzero 6th powers.
 *            DP: compute which numbers are reachable using at most k sixth
 *            powers, for k = 0..10
 * Range:     u8 n <= 38, u16 n <= 3380, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LIMIT 3000000

typedef struct {
    seq_t *values;
    uint64_t count;
} SequenceCache;

static SequenceCache cache = {NULL, 0};

static void compute_sequence(void)
{
    if (cache.values) return;
    
    uint64_t limit = MAX_LIMIT;
    
    // Compute sixth powers up to limit
    uint64_t sixth_powers[50];
    int num_powers = 0;
    for (uint64_t i = 1; i < 100; i++) {
        uint64_t p = i * i * i * i * i * i;
        if (p > limit) break;
        sixth_powers[num_powers++] = p;
    }
    
    // DP arrays: curr[i] = reachable with at most k powers
    uint8_t *curr = (uint8_t *)calloc(limit + 1, sizeof(uint8_t));
    uint8_t *next = (uint8_t *)calloc(limit + 1, sizeof(uint8_t));
    if (!curr || !next) {
        free(curr);
        free(next);
        return;
    }
    
    curr[0] = 1;
    
    // For each number of terms from 1 to 10
    for (int k = 1; k <= 10; k++) {
        memcpy(next, curr, limit + 1);
        
        // For each sixth power, extend reachable numbers
        for (int p = 0; p < num_powers; p++) {
            uint64_t power = sixth_powers[p];
            for (uint64_t i = power; i <= limit; i++) {
                if (curr[i - power]) {
                    next[i] = 1;
                }
            }
        }
        
        memcpy(curr, next, limit + 1);
    }
    
    // Count reachable numbers
    uint64_t count = 0;
    for (uint64_t i = 0; i <= limit; i++) {
        if (curr[i]) count++;
    }
    
    // Store sorted values
    cache.values = (seq_t *)malloc((count + 1) * sizeof(seq_t));
    if (!cache.values) {
        free(curr);
        free(next);
        return;
    }
    
    count = 0;
    for (uint64_t i = 0; i <= limit; i++) {
        if (curr[i]) {
            cache.values[count++] = (seq_t)i;
        }
    }
    
    cache.count = count;
    
    free(curr);
    free(next);
}

seq_t a(idx_t n)
{
    if (!cache.values) {
        compute_sequence();
    }
    
    if (n < 1 || n > cache.count) return (seq_t)0;
    
    return cache.values[n - 1];
}
