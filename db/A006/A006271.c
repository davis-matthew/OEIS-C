/*
 * A006271  Numerators of a continued fraction for 1 + sqrt(2).
 * https://oeis.org/A006271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 2, a(1) = 5, a(n) = a(n-1)^3 + 3*a(n-1)^2 - 3
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 5 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *cache = NULL;
static idx_t cache_capacity = 0;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)5;
    
    if (cache == NULL) {
        cache_capacity = 10;
        cache = mem_new(seq_t, cache_capacity);
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)5;
        cache_len = 2;
    }
    
    while (n >= cache_capacity) {
        idx_t new_capacity = cache_capacity * 2;
        seq_t *new_cache = mem_new(seq_t, new_capacity);
        for (idx_t i = 0; i < cache_len; i++) {
            new_cache[i] = cache[i];
        }
        mem_free(cache);
        cache = new_cache;
        cache_capacity = new_capacity;
    }
    
    for (idx_t i = cache_len; i <= n; i++) {
        seq_t prev = cache[i - 1];
        seq_t prev_sq = prev * prev;
        seq_t prev_cubed = prev_sq * prev;
        cache[i] = prev_cubed + (seq_t)3 * prev_sq - (seq_t)3;
    }
    
    if (cache_len <= n) cache_len = n + 1;
    
    return cache[n];
}
