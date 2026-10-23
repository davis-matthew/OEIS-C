/*
 * A160305  Numerator of Hermite(n, 7/31).
 * https://oeis.org/A160305
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n+2) = 14*a(n+1) - 1922*(n+1)*a(n), a(0) = 1, a(1) = 14
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 9, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for memoization */
static seq_t *cache = NULL;      /* cached sequence values */
static size_t cache_cap = 0;     /* capacity of the cache array */
static idx_t cache_max = (idx_t)-1; /* highest index that has been computed */

/* Ensure that cache contains the sequence up to index n, allocating or
   extending the array as necessary.  Values are computed via the
   recurrence above. */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* Initial allocation: need at least two elements (a0 and a1). */
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        /* Initialise first two terms. */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)14;
        cache_max = 1;
    } else if (n >= (idx_t)cache_cap) {
        /* Grow the array to at least n+1 elements. */
        size_t new_cap = cache_cap;
        while ((idx_t)new_cap <= n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to n. */
    if (cache_max < n) {
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            /* a[i] = 14*a[i-1] - 1922*(i-1)*a[i-2] */
            seq_t term = (seq_t)14 * cache[i - 1];
            term = term - (seq_t)1922 * (seq_t)(i - 1) * cache[i - 2];
            cache[i] = term;
        }
        cache_max = n;
    }
}

/* Public API required by the compiler. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
