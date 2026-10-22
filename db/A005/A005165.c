/*
 * A005165  Alternating factorials: n! - (n-1)! + (n-2)! - ... 1!.
 * https://oeis.org/A005165
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n*a(n-2) + (n-1)*a(n-1) for n > 1, a(0) = 0, a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    
    if (cache == NULL) {
        cache_len = 16;
        cache = (seq_t *)calloc(cache_len, sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 1;
    }
    
    if (n == 0) {
        return 0;
    }
    
    if (n >= cache_len) {
        idx_t new_size = cache_len;
        while (new_size <= n) {
            new_size *= 2;
        }
        
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        memset(cache + cache_len, 0, (new_size - cache_len) * sizeof(seq_t));
        cache_len = new_size;
    }
    
    for (idx_t i = 2; i <= n; i++) {
        if (cache[i] == 0) {
            cache[i] = (seq_t)i * cache[i-2] + (seq_t)(i-1) * cache[i-1];
        }
    }
    
    return cache[n];
}
