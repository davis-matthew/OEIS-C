/*
 * A006502  Related to Fibonacci numbers.
 * https://oeis.org/A006502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 10*a(n-1) - 34*a(n-2) + 32*a(n-3) + 54*a(n-4) - 94*a(n-5) - 50*a(n-6) + 94*a(n-7) + 54*a(n-8) - 32*a(n-9) - 34*a(n-10) - 10*a(n-11) - a(n-12) for n >= 12 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_alloc = 0;
static idx_t cache_computed = 0;

seq_t a(idx_t n) {
    static const seq_t init[12] = {
        1, 3, 14, 60, 279, 1251, 5329, 21532, 83225, 310464, 1126502, 4000602
    };
    
    if (n < 12) {
        return init[n];
    }
    
    if (cache && n < cache_computed) {
        return cache[n];
    }
    
    if (!cache) {
        cache_alloc = (n < 512) ? 512 : n + 256;
        cache = (seq_t *)malloc(cache_alloc * sizeof(seq_t));
        if (!cache) return 0;
        
        for (idx_t i = 0; i < 12; i++) {
            cache[i] = init[i];
        }
        cache_computed = 12;
    } else if (n >= cache_alloc) {
        idx_t new_alloc = n + 256;
        seq_t *new_cache = (seq_t *)malloc(new_alloc * sizeof(seq_t));
        if (!new_cache) return 0;
        
        for (idx_t i = 0; i < cache_computed; i++) {
            new_cache[i] = cache[i];
        }
        free(cache);
        cache = new_cache;
        cache_alloc = new_alloc;
    }
    
    for (idx_t i = cache_computed; i <= n; i++) {
        cache[i] = 10*cache[i-1] - 34*cache[i-2] + 32*cache[i-3] + 54*cache[i-4]
                 - 94*cache[i-5] - 50*cache[i-6] + 94*cache[i-7] + 54*cache[i-8]
                 - 32*cache[i-9] - 34*cache[i-10] - 10*cache[i-11] - cache[i-12];
    }
    cache_computed = n + 1;
    
    return cache[n];
}
