/*
 * A005710  a(n) = a(n-1) + a(n-8), with a(i) = 1 for i = 0..7.
 * https://oeis.org/A005710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-8), a(i) = 1 for i = 0..7
 * Range:     u8 n <= 30, u16 n <= 57, u32 n <= 110, u64 n <= 216,
 *            u128 n <= 429, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    
    if (cache == NULL) {
        cache_len = 8;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        for (idx_t i = 0; i < 8; i++) {
            cache[i] = 1;
        }
    }
    
    if (n < cache_len) {
        return cache[n];
    }
    
    seq_t *new_cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    for (idx_t i = 0; i < cache_len; i++) {
        new_cache[i] = cache[i];
    }
    free(cache);
    
    for (idx_t i = cache_len; i <= n; i++) {
        new_cache[i] = new_cache[i - 1] + new_cache[i - 8];
    }
    
    cache = new_cache;
    cache_len = n + 1;
    
    return cache[n];
}
