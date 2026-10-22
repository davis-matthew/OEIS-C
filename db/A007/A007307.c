/*
 * A007307  a(n) = a(n-2) + a(n-3), with a(0) = 0, a(1) = 1, a(2) = 2.
 * https://oeis.org/A007307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + a(n-3), a(0) = 0, a(1) = 1, a(2) = 2
 * Range:     u8 n <= 20, u16 n <= 40, u32 n <= 79, u64 n <= 158,
 *            u128 n <= 316, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t computed = 0;
    
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    
    if (cache == NULL) {
        cache_size = (n + 1 > 16) ? (n + 1) : 16;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;
        
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        computed = 3;
    }
    
    if (n >= cache_size) {
        idx_t new_size = cache_size * 2;
        while (new_size <= n) new_size *= 2;
        
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (new_cache == NULL) return (seq_t)0;
        
        memcpy(new_cache, cache, computed * sizeof(seq_t));
        free(cache);
        cache = new_cache;
        cache_size = new_size;
    }
    
    for (idx_t i = computed; i <= n; i++) {
        cache[i] = cache[i-2] + cache[i-3];
    }
    computed = n + 1;
    
    return cache[n];
}
