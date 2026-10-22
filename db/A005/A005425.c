/*
 * A005425  a(n) = 2*a(n-1) + (n-1)*a(n-2).
 * https://oeis.org/A005425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 2*a(n-1) + (n-1)*a(n-2), a(0) = 1, a(1) = 2
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 29, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t max_computed = 0;
    
    if (cache == NULL) {
        cache_len = (n > 1) ? n + 1 : 2;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = 2;
        max_computed = 1;
    }
    
    if (n >= cache_len) {
        idx_t new_len = (n + 1) * 2;
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        for (idx_t i = 0; i < cache_len; i++) {
            new_cache[i] = cache[i];
        }
        free(cache);
        cache = new_cache;
        cache_len = new_len;
    }
    
    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; i++) {
            cache[i] = 2 * cache[i-1] + (seq_t)(i - 1) * cache[i-2];
        }
        max_computed = n;
    }
    
    return cache[n];
}
