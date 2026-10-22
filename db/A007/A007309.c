/*
 * A007309  a(n) = a(n-2) + a(n-3), with a(0) = 0, a(1) = 1, a(2) = 4.
 * https://oeis.org/A007309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + a(n-3), a(0) = 0, a(1) = 1, a(2) = 4
 * Range:     u8 n <= 18, u16 n <= 38, u32 n <= 78, u64 n <= 156,
 *            u128 n <= 314, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_computed = 0;
static idx_t cache_allocated = 0;

seq_t a(idx_t n) {
    if (cache == NULL) {
        cache_allocated = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_allocated);
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 4;
        cache_computed = 3;
    }
    
    if (n < cache_computed) {
        return cache[n];
    }
    
    if (n >= cache_allocated) {
        idx_t new_allocated = cache_allocated * 2;
        while (new_allocated <= n) {
            new_allocated *= 2;
        }
        seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_allocated);
        for (idx_t i = 0; i < cache_computed; i++) {
            new_cache[i] = cache[i];
        }
        free(cache);
        cache = new_cache;
        cache_allocated = new_allocated;
    }
    
    for (idx_t i = cache_computed; i <= n; i++) {
        cache[i] = cache[i - 2] + cache[i - 3];
    }
    cache_computed = n + 1;
    
    return cache[n];
}
