/*
 * A015441  Generalized Fibonacci numbers.
 * https://oeis.org/A015441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = a(n-1) + 6*a(n-2), a(0)=0, a(1)=1
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 846 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses a 2‑term recurrence; cache is reused across calls. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

/* Helper: allocate an array of seq_t and zero it. */
static seq_t *alloc_seq(size_t n)
{
    return mem_new(seq_t, n);
}

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

static seq_t *cache = NULL;         /* cached values */
static size_t cache_cap = 0;        /* current capacity of cache array   */
static idx_t max_cached = 0;        /* highest n already computed in cache */

/* Computes A015441 entry a(n). */
seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Ensure we have room for at least n+1 entries. */
    ensure_cache(n + 1, &cache, &cache_cap);

    /* Initialise first two entries if required. */
    if (max_cached < 1) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        max_cached = 1;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        cache[i] = cache[i - 1] + cache[i - 2] * (seq_t)6;
    }
    max_cached = n;

    return cache[n];
}
