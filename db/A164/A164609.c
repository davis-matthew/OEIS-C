/*
 * A164609  a(n) = 10*a(n-1) - 17*a(n-2) for n > 1; a(0) = 1, a(1) = 13.
 * https://oeis.org/A164609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - 17*a(n-2) for n > 1; a(0) = 1, a(1) = 13
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 974 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
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
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t last   = 1;        /* largest index already computed */

    /* Ensure there is enough space for index n */
    ensure_cache(n, &cache, &cap);

    /* Initialise base values once */
    if (cache[0] == 0) cache[0] = (seq_t)1;
    if (cache[1] == 0) cache[1] = (seq_t)13;

    if (n < 2) return cache[n];

    /* Compute missing terms */
    if (last < n) {
        for (idx_t i = last + 1; i <= n; ++i) {
            /* All operations are performed in seq_t; this is safe
               because we only require exactness when the result fits */
            cache[i] = cache[i-1] * (seq_t)10 - cache[i-2] * (seq_t)17;
        }
        last = n;
    }
    return cache[n];
}
