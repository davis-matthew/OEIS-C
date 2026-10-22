/*
 * A007420  Berstel sequence: a(n+1) = 2*a(n) - 4*a(n-1) + 4*a(n-2).
 * https://oeis.org/A007420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, nice
 * Formula:   a(n) = 2*a(n-1) - 4*a(n-2) + 4*a(n-3), a(0) = 0, a(1) = 0, a(2)
 *            = 1
 * Range:     i8 n <= 11, i16 n <= 20, i32 n <= 41, i64 n <= 80, i128 n <= 158,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>


static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t highest_computed = 2;

seq_t a(idx_t n) {
    if (cache == NULL) {
        cache_len = 10;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 1;
    }
    
    while (n >= cache_len) {
        seq_t *new_cache = (seq_t *)malloc(cache_len * 2 * sizeof(seq_t));
        memcpy(new_cache, cache, cache_len * sizeof(seq_t));
        free(cache);
        cache = new_cache;
        cache_len *= 2;
    }
    
    if (n > highest_computed) {
        for (idx_t i = highest_computed + 1; i <= n; i++) {
            cache[i] = 2 * cache[i-1] - 4 * cache[i-2] + 4 * cache[i-3];
        }
        highest_computed = n;
    }
    
    return cache[n];
}
