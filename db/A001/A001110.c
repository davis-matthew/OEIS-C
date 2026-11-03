/*
 * A001110  Square triangular numbers: numbers that are both triangular and
 *          square.
 * https://oeis.org/A001110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 34*a(n-1) - a(n-2) + 2, a(0) = 0, a(1) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 594 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    
    if (cache_len == 0) {
        cache = mem_new(seq_t, 256);
        cache[0] = 0;
        cache[1] = 1;
        cache_len = 2;
    }
    
    if (n < cache_len) {
        return cache[n];
    }
    
    idx_t new_len = cache_len;
    while (new_len <= n) {
        new_len *= 2;
    }
    cache = mem_resize(seq_t, cache, new_len);
    
    for (idx_t i = cache_len; i <= n; i++) {
        cache[i] = 34 * cache[i-1] - cache[i-2] + 2;
    }
    cache_len = n + 1;
    
    return cache[n];
}
