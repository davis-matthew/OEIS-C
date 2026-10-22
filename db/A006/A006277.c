/*
 * A006277  a(n) = (a(n-1) + 1)*a(n-2).
 * https://oeis.org/A006277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (a(n-1) + 1)*a(n-2), a(0) = 1, a(1) = 1
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 8, u64 n <= 10, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    // Initialize cache if needed
    if (cache == NULL) {
        cache_len = (n < 16) ? 16 : (n + 1);
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = (seq_t)1;
        if (cache_len > 1) {
            cache[1] = (seq_t)1;
        }
        for (idx_t i = 2; i < cache_len; i++) {
            cache[i] = (cache[i-1] + (seq_t)1) * cache[i-2];
        }
    }
    
    // Extend cache if needed
    if (n >= cache_len) {
        idx_t new_len = (n + 1 > cache_len * 2) ? (n + 1) : (cache_len * 2);
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        
        for (idx_t i = 0; i < cache_len; i++) {
            new_cache[i] = cache[i];
        }
        
        for (idx_t i = cache_len; i < new_len; i++) {
            new_cache[i] = (new_cache[i-1] + (seq_t)1) * new_cache[i-2];
        }
        
        free(cache);
        cache = new_cache;
        cache_len = new_len;
    }
    
    return cache[n];
}
