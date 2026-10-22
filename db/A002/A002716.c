/*
 * A002716  An infinite coprime sequence defined by recursion.
 * https://oeis.org/A002716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(2*n) = a(2*n-1)^2 - 3*a(2*n-1) + 3, a(2*n+1) = a(2*n) +
 *            a(2*n-1) - 1, a(0) = 3, a(1) = 5
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 10, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static seq_t *cache = NULL;
static idx_t cache_capacity = 0;
static idx_t cache_computed = 0;

seq_t a(idx_t n) {
    if (n >= cache_capacity) {
        idx_t new_capacity = (n + 1 < 16) ? 16 : n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_capacity * sizeof(seq_t));
        
        if (cache != NULL) {
            for (idx_t i = 0; i < cache_capacity; i++) {
                new_cache[i] = cache[i];
            }
            free(cache);
        }
        
        cache = new_cache;
        cache_capacity = new_capacity;
    }
    
    if (cache_computed == 0) {
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)5;
        cache_computed = 2;
    }
    
    for (idx_t i = cache_computed; i <= n; i++) {
        if (i % 2 == 0) {
            seq_t prev = cache[i - 1];
            cache[i] = prev * prev - (seq_t)3 * prev + (seq_t)3;
        } else {
            cache[i] = cache[i - 1] + cache[i - 2] - (seq_t)1;
        }
    }
    
    if (n + 1 > cache_computed) {
        cache_computed = n + 1;
    }
    
    return cache[n];
}
