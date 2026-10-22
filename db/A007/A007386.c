/*
 * A007386  Number of strict 7th-order maximal independent sets in path graph.
 * https://oeis.org/A007386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-2) - 3*a(n-4) + a(n-6) + a(n-9) - 2*a(n-11) +
 *            a(n-13) with a(1)..a(8) = 0, a(9) = 1, a(10) = 0, a(11) = 3,
 *            a(12) = 0, a(13) = 6
 * Range:     u8 n <= 37, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t max_computed = 0;
    
    static const seq_t base[] = {
        0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 3, 0, 6
    };
    
    if (n <= 13) {
        return base[n - 1];
    }
    
    if (cache == NULL || cache_size < n) {
        idx_t new_size = (cache_size == 0) ? 
                         ((n < 1000) ? 1000 : n + 100) :
                         (n + 100);
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        
        if (cache != NULL) {
            for (idx_t i = 0; i < cache_size; i++) {
                new_cache[i] = cache[i];
            }
            free(cache);
        } else {
            for (idx_t i = 0; i < 13; i++) {
                new_cache[i] = base[i];
            }
            max_computed = 13;
        }
        
        cache = new_cache;
        cache_size = new_size;
    }
    
    while (max_computed < n) {
        idx_t i = max_computed + 1;
        seq_t val = (seq_t)3 * cache[i - 3]
                  - (seq_t)3 * cache[i - 5]
                  + cache[i - 7]
                  + cache[i - 10]
                  - (seq_t)2 * cache[i - 12]
                  + cache[i - 14];
        cache[i - 1] = val;
        max_computed = i;
    }
    
    return cache[n - 1];
}
