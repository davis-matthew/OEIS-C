/*
 * A083295  a(n) = (4*2^n + (-8)^n)/5.
 * https://oeis.org/A083295
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (4*2^n + (-8)^n)/5 Recurrence: a(0)=1, a(1)=0, a(n)=
 *            -6*a(n-1) + 16*a(n-2) for n≥2
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 11, i64 n <= 21, i128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached terms: cache_max = number of terms already computed (n+1) */
static idx_t cache_alloc = 0;      /* allocated size of cache */
static idx_t cache_max   = 0;      /* number of terms computed */
static seq_t *cache = NULL;

/* Grow the cache so that it can hold at least indices 0..n */
static void ensure_cache_alloc(idx_t n)
{
    idx_t needed = n + 1;          /* we need index n, so size = n+1 */
    if (cache_alloc < needed) {
        idx_t new_alloc = (cache_alloc == 0) ? 1 : cache_alloc;
        while (new_alloc < needed)
            new_alloc <<= 1;      /* double until big enough */
        cache = (seq_t*)realloc(cache, new_alloc * sizeof(seq_t));
        cache_alloc = new_alloc;
    }
}

seq_t a(idx_t n)
{
    /* Ensure base terms are present */
    if (cache_max == 0) {
        ensure_cache_alloc(1);    /* need indices 0 and 1 */
        cache[0] = 1;
        cache[1] = 0;
        cache_max = 2;
    }

    /* Return cached value if already computed */
    if (n < cache_max)
        return cache[n];

    /* Compute new terms up to n */
    ensure_cache_alloc(n);
    for (idx_t i = cache_max; i <= n; ++i) {
        cache[i] = -6 * cache[i-1] + 16 * cache[i-2];
    }
    cache_max = n + 1;            /* update number of computed terms */
    return cache[n];
}
