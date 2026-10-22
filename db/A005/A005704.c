/*
 * A005704  Number of partitions of 3n into powers of 3.
 * https://oeis.org/A005704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(floor(n/3)), a(0) = 1
 * Range:     u8 n <= 27, u16 n <= 160, u32 n <= 1691, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    
    if (cache == NULL) {
        cache_len = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        cache[0] = 1;
    }
    
    if (n >= cache_len) {
        idx_t old_len = cache_len;
        cache_len = n + 1;
        cache = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
        
        for (idx_t i = old_len; i <= n; i++) {
            cache[i] = cache[i - 1] + cache[i / 3];
        }
    }
    
    return cache[n];
}
