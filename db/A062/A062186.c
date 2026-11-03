/*
 * A062186  a(n) = a(n-1) - a(floor(n/2)), with a(1)=1.
 * https://oeis.org/A062186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-1) - a(floor(n/2)), a(1) = 1
 * Range:     i8 n <= 47, i16 n <= 70, i32 n <= 70, i64 n <= 70, i128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

/* a(n) is defined for n >= 1 only. */
static seq_t *cache = NULL;            /* 1‑based cache for already computed terms   */
static idx_t cache_max = 0;            /* highest n that has been computed          */

/* Extend the cache up to at least n and compute all missing terms. */
static void compute_up_to(idx_t n)
{
    if (n <= cache_max) return;

    /* Allocate or reallocate the cache. */
    if (!cache)
        cache = mem_new(seq_t, (n + 1));
    else
        cache = mem_resize(seq_t, cache, (n + 1));

    /* Initialize starting terms if this is the first call. */
    if (cache_max == 0) {
        cache[1] = (seq_t)1;
        cache_max = 1;
    }

    /* Compute terms from cache_max+1 up to n. */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        idx_t half = i / 2;
        seq_t half_val = cache[half];
        cache[i] = prev - half_val;
    }

    cache_max = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, return 0 for safety   */
    compute_up_to(n);
    return cache[n];
}
