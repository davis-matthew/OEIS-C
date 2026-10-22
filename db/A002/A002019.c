/*
 * A002019  a(n) = a(n-1) - (n-1)(n-2)a(n-2).
 * https://oeis.org/A002019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, nice, easy
 * Formula:   a(n) = a(n-1) - (n-1)(n-2)a(n-2)
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 13, i64 n <= 21, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n) {
    if (cache_size == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t) * 2);
        cache[0] = 1;
        cache[1] = 1;
        cache_size = 2;
    }
    
    if (n >= cache_size) {
        idx_t new_size = cache_size;
        while (new_size <= n) {
            new_size *= 2;
        }
        seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_size);
        for (idx_t i = 0; i < cache_size; i++) {
            new_cache[i] = cache[i];
        }
        free(cache);
        cache = new_cache;
        
        for (idx_t i = cache_size; i <= n; i++) {
            seq_t si = (seq_t)i;
            seq_t term = (si - 1) * (si - 2) * cache[i - 2];
            cache[i] = cache[i - 1] - term;
        }
        cache_size = n + 1;
    }
    
    return cache[n];
}
