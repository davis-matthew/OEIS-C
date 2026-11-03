/*
 * A157176  a(n+1) = a(n - n mod 2) + a(n - n mod 3), a(0) = 1.
 * https://oeis.org/A157176
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+1) = a(n - n mod 2) + a(n - n mod 3), a(0) = 1
 * Range:     u8 n <= 15, u16 n <= 30, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 44 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* static cache for computed terms (0..N) */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity */
    static size_t computed   = 0;   /* number of terms currently filled */

    /* Grow cache if necessary */
    if (cache == NULL) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
    }
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while ((size_t)n + 1 > new_cap) new_cap <<= 1;
        cache = mem_resize(seq_t, cache, new_cap);
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (size_t i = computed; i <= (size_t)n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else {
            idx_t prev   = (idx_t)i - 1;
            size_t idx1 = (size_t)(prev - (prev % 2));
            size_t idx2 = (size_t)(prev - (prev % 3));
            cache[i] = cache[idx1] + cache[idx2];
        }
    }
    computed = (size_t)n + 1;
    return cache[(size_t)n];
}
