/*
 * A001584  A generalized Fibonacci sequence.
 * https://oeis.org/A001584
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-3) - a(n-6) + a(n-8), a(0)..a(7) = 1
 * Range:     u8 n <= 32, u16 n <= 60, u32 n <= 116, u64 n <= 227,
 *            u128 n <= 449, big any n
 * Validated: standard, pass, 76 terms through n = 923 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"


seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    
    if (cache == NULL) {
        cache_len = 8;
        cache = mem_new(seq_t, cache_len);
        for (idx_t i = 0; i < 8; i++) {
            cache[i] = (seq_t)1;
        }
    }
    
    if (n < cache_len) {
        return cache[n];
    }
    
    seq_t *new_cache = mem_new(seq_t, (n + 1));
    for (idx_t i = 0; i < cache_len; i++) {
        new_cache[i] = cache[i];
    }
    
    for (idx_t i = cache_len; i <= n; i++) {
        new_cache[i] = (seq_t)2 * new_cache[i - 3] - new_cache[i - 6] + new_cache[i - 8];
    }
    
    mem_free(cache);
    cache = new_cache;
    cache_len = n + 1;
    
    return cache[n];
}
