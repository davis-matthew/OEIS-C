/*
 * A159657  Numerator of Hermite(n, 1/20).
 * https://oeis.org/A159657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = a(n-1) - 200*(n-1)*a(n-2) a(0) = 1, a(1) = 1 (OEIS
 *            A159657: Numerator of Hermite(n, 1/20))
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 7, i64 n <= 13, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Static cache for memoisation */
static seq_t *cache = NULL;     /* array of computed terms */
static idx_t  cache_len = 0;    /* number of terms stored currently */
static idx_t  cache_cap = 0;    /* current allocation capacity */

/* Ensure that cache contains terms up to index n (inclusive). */
static void
ensure_cache(idx_t n)
{
    if (n < cache_len)            /* already computed */
        return;

    /* Grow capacity if necessary (doubling strategy). */
    idx_t new_cap = cache_cap > 0 ? cache_cap : 2;
    while (new_cap <= n)
        new_cap <<= 1;

    if (cache == NULL) {
        cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    } else {
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    }
    cache_cap = new_cap;

    /* Initialise first terms if this is the first call. */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        if (n >= 1) {
            cache[1] = (seq_t)1;
            cache_len = 2;
        } else {
            cache_len = 1;
        }
    }

    /* Compute remaining terms up to n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t prev      = cache[i - 1];
        seq_t prevprev  = cache[i - 2];
        seq_t term = prev - (seq_t)200 * (seq_t)(i - 1) * prevprev;
        cache[i] = term;
    }
    cache_len = n + 1;
}

/* Public entry point for the sequence. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
