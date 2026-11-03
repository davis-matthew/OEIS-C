/*
 * A001696  a(n) = a(n-1)*(1 + a(n-1) - a(n-2)), a(0) = 0, a(1) = 1.
 * https://oeis.org/A001696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)*(1 + a(n-1) - a(n-2)), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Validated: standard, pass, 14 terms through n = 13 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    
    if (n == 0) return 0;
    if (n == 1) return 1;
    
    if (cache == NULL) {
        cache = mem_new(seq_t, 2);
        cache[0] = 0;
        cache[1] = 1;
        cache_len = 2;
    }
    
    if (n >= cache_len) {
        cache = mem_resize(seq_t, cache, (n + 1));
        for (idx_t i = cache_len; i <= n; i++) {
            seq_t prev = cache[i - 1];
            seq_t prev_prev = cache[i - 2];
            cache[i] = prev * (1 + prev - prev_prev);
        }
        cache_len = n + 1;
    }
    
    return cache[n];
}
