/*
 * A003999  Sums of distinct nonzero 4th powers.
 * https://oeis.org/A003999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   sums of distinct nonzero 4th powers; for n > 4244664, a(n) = n +
 *            889576
 * Range:     u8 n <= 7, u16 n <= 7239, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>


seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    
    if (cache == NULL) {
        uint64_t max_val = 10000000ULL;
        
        bool *reachable = (bool *)calloc((size_t)(max_val + 1), sizeof(bool));
        reachable[0] = true;
        
        for (uint64_t base = 1; base * base * base * base <= max_val; base++) {
            uint64_t power = base * base * base * base;
            
            for (uint64_t sum = max_val; sum >= power; sum--) {
                if (reachable[sum - power]) {
                    reachable[sum] = true;
                }
            }
        }
        
        idx_t sum_count = 0;
        for (uint64_t i = 1; i <= max_val; i++) {
            if (reachable[i]) {
                sum_count++;
            }
        }
        
        cache = (seq_t *)malloc((size_t)sum_count * sizeof(seq_t));
        cache_len = 0;
        for (uint64_t i = 1; i <= max_val; i++) {
            if (reachable[i]) {
                cache[cache_len++] = (seq_t)i;
            }
        }
        
        free(reachable);
    }
    
    if (n <= cache_len) {
        return cache[n - 1];
    } else if (n > 4244664) {
        return (seq_t)n + (seq_t)889576;
    } else {
        return 0;
    }
}
