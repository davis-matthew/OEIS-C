/*
 * A121258  a(n) = a(n-1)*a(n-2)*a(n-3) - 1 with a(0)=a(1)=a(2)=2.
 * https://oeis.org/A121258
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)*a(n-2)*a(n-3) - 1; a(0)=a(1)=a(2)=2
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Validated: standard, pass, 15 terms through n = 14 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t max_computed = 0;

    if (cache == NULL) {
        /* Allocate space for the first three values */
        ensure_cache(2, &cache, &cache_cap);
        cache[0] = cache[1] = cache[2] = (seq_t)2;
        max_computed = 2;
    }

    if (n <= max_computed) {
        return cache[n];
    }

    /* Expand cache if necessary */
    ensure_cache(n, &cache, &cache_cap);

    /* Compute new values up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        cache[i] = cache[i-1] * cache[i-2] * cache[i-3] - (seq_t)1;
    }

    max_computed = n;
    return cache[n];
}
