/*
 * A015575  Expansion of x/(1 - 8*x - 5*x^2).
 * https://oeis.org/A015575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) + 5*a(n-2), with a(0) = 0, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 871 (2026-11-03)
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
    static size_t cap = 0;
    static size_t computed = 0;          /* highest index computed so far */

    /* Make sure the cache can hold indices up to n. */
    ensure_cache(n, &cache, &cap);

    /* Initialise the first terms if this is the first call. */
    if (computed == 0) {
        cache[0] = (seq_t)0;
        if (n >= 1) {
            cache[1] = (seq_t)1;
            computed = 1;
        }
    }

    /* Compute any missing terms up to n. */
    while (computed < n) {
        size_t i = computed + 1;
        cache[i] = 8 * cache[i - 1] + 5 * cache[i - 2];
        computed = i;
    }

    return cache[n];
}
