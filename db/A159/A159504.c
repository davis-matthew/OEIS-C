/*
 * A159504  Numerator of Hermite(n, 10/13).
 * https://oeis.org/A159504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 13^n * Hermite(n, 10/13) Recurrence: a(0) = 1, a(1) = 20,
 *            a(n) = 20 * a(n-1) - 338 * (n-1) * a(n-2) (n ≥ 2) Computation
 *            uses a simple memoised recurrence, no floating‑point or special
 *            utilities
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 6, i64 n <= 11, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

/* Static cache for computed values */
static seq_t *cache = NULL;        /* array of cached a(i) */
static idx_t  cache_len = 0;       /* number of elements currently filled (0 … cache_len‑1) */
static size_t cache_cap = 0;       /* allocated capacity of 'cache' */

/* Grow the cache to hold at least indices up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if ((size_t)n + 1 <= cache_cap)          /* already large enough */
        return;

    size_t new_cap = (size_t)n + 1;
    seq_t *new_cache = NULL;

    if (cache) {                            /* reuse existing storage */
        new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    } else {                                 /* first allocation */
        new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    }
    if (!new_cache)                          /* allocation failed – leave cache as is */
        return;

    cache = new_cache;
    cache_cap = new_cap;

    /* Compute missing values */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0)
            cache[i] = (seq_t)1;
        else if (i == 1)
            cache[i] = (seq_t)20;
        else {
            seq_t term1 = (seq_t)20 * cache[i - 1];
            seq_t term2 = (seq_t)338 * (seq_t)(i - 1) * cache[i - 2];
            cache[i] = term1 - term2;
        }
    }

    cache_len = n + 1;
}

/* Return a(n) for any n ≥ 0, in any order of calls. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
