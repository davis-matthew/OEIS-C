/*
 * A006496  Imaginary part of (1+2i)^n.
 * https://oeis.org/A006496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*a(n-1) - 5*a(n-2); a(0) = 0, a(1) = 2
 * Range:     i8 n <= 6, i16 n <= 12, i32 n <= 26, i64 n <= 54, i128 n <= 109,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    if (cache == NULL) {
        cache_len = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        cache[0] = 0;
        cache[1] = 2;
    }
    
    if (n < cache_len) {
        return cache[n];
    }
    
    seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
    for (idx_t i = 0; i < cache_len; i++) {
        new_cache[i] = cache[i];
    }
    free(cache);
    cache = new_cache;
    
    for (idx_t i = cache_len; i <= n; i++) {
        cache[i] = 2 * cache[i-1] - 5 * cache[i-2];
    }
    cache_len = n + 1;
    
    return cache[n];
}
