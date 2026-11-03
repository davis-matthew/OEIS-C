/*
 * A006357  Number of antichains in the product of a fence poset with n
 *          elements and a chain with 3 elements; also number of paths with n
 *          turns when light is reflected from 4 glass plates.
 * https://oeis.org/A006357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-2) - a(n-3) - a(n-4), a(0) = 1, a(1) =
 *            4, a(2) = 10, a(3) = 30
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 83,
 *            big any n
 * Validated: standard, pass, 44 terms through n = 199 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t computed_up_to = 0;
    
    if (cache == NULL) {
        cache_len = (n > 255) ? (n + 1) : 256;
        cache = mem_new(seq_t, cache_len);
        cache[0] = 1;
        cache[1] = 4;
        cache[2] = 10;
        cache[3] = 30;
        computed_up_to = 3;
    }
    
    if (n >= cache_len) {
        idx_t new_len = cache_len;
        while (new_len <= n) new_len *= 2;
        cache = mem_resize(seq_t, cache, new_len);
        cache_len = new_len;
    }
    
    if (n > computed_up_to) {
        for (idx_t i = computed_up_to + 1; i <= n; i++) {
            cache[i] = 2 * cache[i-1] + 3 * cache[i-2] - cache[i-3] - cache[i-4];
        }
        computed_up_to = n;
    }
    
    return cache[n];
}
