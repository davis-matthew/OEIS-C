/*
 * A182415  a(0) = 1, a(1) = 2; for n>1, a(n) = a(n-1) + a(n-2) + 4.
 * https://oeis.org/A182415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + 4, a(0)=1, a(1)=2
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Validated: standard, pass, 54 terms through n = 984 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Initialise the first two terms if required */
    if (cache == NULL) {
        cache_size = 2;
        cache = mem_new(seq_t, cache_size);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
    }

    /* Grow the cache and compute new values if n is beyond what we have */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        cache = mem_resize(seq_t, cache, new_size);
        /* Compute the missing terms */
        for (idx_t i = cache_size; i <= n; ++i) {
            cache[i] = cache[i - 1] + cache[i - 2] + (seq_t)4;
        }
        cache_size = new_size;
    }

    return cache[n];
}
