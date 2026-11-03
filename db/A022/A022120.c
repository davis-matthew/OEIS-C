/*
 * A022120  Fibonacci sequence beginning 3, 7.
 * https://oeis.org/A022120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 3, a(1) = 7; a(n) = a(n-1) + a(n-2) for n ≥ 2
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Validated: standard, pass, 53 terms through n = 4287 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS sequence A022120: Fibonacci-like recurrence with
   initial terms 3, 7. */

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

static seq_t *cache = NULL;   /* array of computed values */
static size_t cache_len = 0;  /* number of values stored in cache */
static size_t cache_cap = 0;  /* current allocated capacity (number of slots) */

seq_t a(idx_t n)
{
    /* Lazy initialization of the first two terms. */
    if (cache_len == 0) {
        /* Ensure space for indices 0 and 1. */
        ensure_cache(1, &cache, &cache_cap);   /* covers indices [0,1] */
        cache[0] = 3;
        cache[1] = 7;
        cache_len = 2;
    }

    /* If the requested index is already cached, return it. */
    if (n < cache_len)
        return cache[n];

    /* Ensure capacity for index n. */
    ensure_cache(n, &cache, &cache_cap);

    /* Compute terms up to n using the recurrence. */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = cache[i - 1] + cache[i - 2];
    }
    cache_len = n + 1;   /* update the number of computed values */

    return cache[n];
}
