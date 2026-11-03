/*
 * A202879  E.g.f.: exp(4*x + x^2/2).
 * https://oeis.org/A202879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Validated: partial, pass, 24 terms through n = 23 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=4, a(n)=4*a(n-1)+(n-1)*a(n-2) for n>=2 */

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

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;  /* number of valid entries in cache */
    static size_t cache_cap  = 0;  /* capacity of cache */

    if (n >= cache_size) {
        /* make sure the cache is large enough */
        ensure_cache(n, &cache, &cache_cap);

        /* compute and store missing entries */
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else if (i == 1) {
                cache[i] = (seq_t)4;
            } else {
                seq_t term1 = (seq_t)4 * cache[i - 1];
                seq_t term2 = (seq_t)(i - 1) * cache[i - 2];
                cache[i] = term1 + term2;
            }
        }
        cache_size = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
