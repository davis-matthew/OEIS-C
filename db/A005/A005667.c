/*
 * A005667  Numerators of continued fraction convergents to sqrt(10).
 * https://oeis.org/A005667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 6*a(n-1) + a(n-2), a(0) = 1, a(1) = 3
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 3;
    
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_high = 1;
    
    if (cache == NULL) {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = 3;
    }
    
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n) {
            new_cap *= 2;
        }
        
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        for (idx_t i = 0; i < cache_cap; i++) {
            new_cache[i] = cache[i];
        }
        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }
    
    for (idx_t i = cache_high + 1; i <= n; i++) {
        cache[i] = 6 * cache[i-1] + cache[i-2];
    }
    
    if (n > cache_high) {
        cache_high = n;
    }
    
    return cache[n];
}
