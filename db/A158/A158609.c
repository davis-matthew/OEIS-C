/*
 * A158609  Expansion of (1+8*x)/(1-x-81*x^2).
 * https://oeis.org/A158609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 81*a(n-2), a(0) = 1, a(1) = 9
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 18 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
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
    static size_t computed = 0;  /* number of terms currently stored */

    /* Initialise the cache on first call */
    if (cache == NULL)
    {
        ensure_cache(2, &cache, &cap);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)9;
        computed = 2;
    }

    /* Make sure we can address cache[n] */
    if ((size_t)n + 1 > cap)
        ensure_cache((size_t)n + 1, &cache, &cap);

    /* Compute any missing terms up to n */
    if ((size_t)n >= computed)
    {
        for (size_t i = computed; i <= (size_t)n; ++i)
        {
            seq_t prod = cache[i - 2] * 81u;
            cache[i] = cache[i - 1] + prod;
        }
        computed = (size_t)n + 1;
    }

    return cache[n];
}
