/*
 * A160280  Numerator of Hermite(n, 18/29).
 * https://oeis.org/A160280
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0)=1, a(1)=36, a(n)=36*a(n-1)-1682*(n-1)*a(n-2) for n≥2 (a(n)
 *            is the integer numerator of Hermite(n,18/29))
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 9, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for previously computed values  */
static seq_t *cache      = NULL;   /* allocated array a[0 … cached_max]   */
static idx_t  cached_max = 1;     /* highest index that has been computed */
static size_t cache_cap  = 0;     /* actual allocated capacity           */

/* Expand the cache so that it contains values up to n (inclusive). */
static void ensure_cached(idx_t n)
{
    /* Initialise the cache on the first call. */
    if (cache == NULL) {
        cache_cap   = 2;
        cache       = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();            /* out of memory */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)36;
        cached_max = 1;
    }

    if (n <= cached_max) return;           /* nothing to do */

    /* Grow the allocated storage if necessary. */
    size_t need = (size_t)(n + 1);
    if (cache_cap < need) {
        size_t new_cap = cache_cap;
        while (new_cap < need) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (cache == NULL) abort();
        cache_cap = new_cap;
    }

    /* Compute new terms using the recurrence. */
    for (idx_t k = cached_max + 1; k <= n; ++k) {
        seq_t term = (seq_t)1682 * (seq_t)(k - 1);
        term *= cache[k - 2];
        cache[k] = (seq_t)36 * cache[k - 1] - term;
    }
    cached_max = n;
}

/* Public function required by the evaluation harness. */
seq_t a(idx_t n)
{
    ensure_cached(n);
    return cache[n];
}
