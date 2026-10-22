/*
 * A005314  For n = 0, 1, 2, a(n) = n; thereafter, a(n) = 2*a(n-1) - a(n-2) +
 *          a(n-3).
 * https://oeis.org/A005314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3), a(0) = 0, a(1) = 1, a(2) = 2
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 40, u64 n <= 79, u128 n <= 158,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t cache_computed = 0;

seq_t a(idx_t n) {
    if (n < 3) {
        return (seq_t)n;
    }
    
    if (cache == NULL) {
        cache_size = 64;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 2;
        cache_computed = 3;
    }
    
    if (n >= cache_size) {
        idx_t new_size = (n + 1) * 2;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        cache_size = new_size;
    }
    
    for (idx_t i = cache_computed; i <= n; i++) {
        cache[i] = 2 * cache[i - 1] - cache[i - 2] + cache[i - 3];
    }
    cache_computed = n + 1;
    
    return cache[n];
}
