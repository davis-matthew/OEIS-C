/*
 * A079929  a(n)=(3*n+1)!/(n!*3^n).
 * https://oeis.org/A079929
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3*n+1)!/(n!*3^n) Recurrence: a(0)=1; a(n) =
 *            (3*n-1)*(3*n+1)*a(n-1) for n>=1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Validated: partial, pass, 13 terms through n = 12 (2026-11-03)
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

static seq_t* cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_size = 0;   // largest index computed so far

seq_t a(idx_t n)
{
    /* Initialise cache on first call */
    if (cache == NULL) {
        ensure_cache(1, &cache, &cache_cap);   /* Ensure space for a(0) */
        cache[0] = 1;
        cache_size = 0;
    }

    /* Return cached value if already computed */
    if (n <= cache_size)
        return cache[n];

    /* Ensure enough capacity for indices up to n */
    ensure_cache(n + 1, &cache, &cache_cap);

    /* Compute missing values up to n */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t factor = (seq_t)(3*i - 1) * (seq_t)(3*i + 1);
        cache[i] = cache[i-1] * factor;
    }

    cache_size = n;
    return cache[n];
}
