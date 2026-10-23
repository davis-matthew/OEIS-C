/*
 * A022398  Fibonacci sequence beginning 1, 28.
 * https://oeis.org/A022398
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2), a(0) = 1, a(1) = 28
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 40, u64 n <= 86, u128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* G.f.: (1+27*x)/(1-x-x^2). */

#include <stdlib.h>

static seq_t *cache = NULL;   /* dynamic cache of computed terms */
static size_t cache_cap = 0;  /* current capacity of cache */

/* Ensure that cache has entries up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (cache_cap > n) return;                 /* nothing to do */

    size_t new_cap = (size_t)n + 1;            /* indices 0..n inclusive */
    if (cache_cap == 0) {
        cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
    } else {
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    }
    /* Compute missing terms */
    for (idx_t i = cache_cap; i <= n; ++i) {
        if (i == 0) {
            cache[0] = 1;
        } else if (i == 1) {
            cache[1] = 28;
        } else {
            cache[(size_t)i] = cache[(size_t)(i-1)] + cache[(size_t)(i-2)];
        }
    }
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
