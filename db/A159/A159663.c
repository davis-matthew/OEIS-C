/*
 * A159663  Numerator of Hermite(n, 11/20).
 * https://oeis.org/A159663
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 11*a(n-1) - 200*(n-1)*a(n-2), with a(0) = 1, a(1) = 11
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 6, i64 n <= 12, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

static seq_t *cache = NULL;   /* Cached terms */
static idx_t  cache_len = 0;  /* Current size of cache */
static idx_t  last_computed = 1; /* Highest index computed (at least 1) */

/* Ensure cache contains terms up to n, computing any missing ones. */
static void ensure_cache(idx_t n)
{
    /* Allocate or resize the cache if necessary. */
    if (cache == NULL) {
        size_t needed = (size_t)(n >= 1 ? n + 1 : 2);
        cache = (seq_t *)calloc(needed, sizeof(seq_t));
        if (!cache) abort();           /* out of memory */
        /* Store the first two terms. */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)11;
        cache_len = needed;
        last_computed = 1;
    } else if (n >= cache_len) {
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_len = new_len;
        /* Remaining slots will be filled on demand. */
    }

    /* Compute any missing terms up to n. */
    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            /* term = 11*a(i-1) - 200*(i-1)*a(i-2) */
            seq_t part = cache[i - 2];
            part *= (seq_t)(i - 1);   /* (i-1)*a(i-2) */
            part *= (seq_t)200;       /* 200*(i-1)*a(i-2) */
            seq_t term = cache[i - 1];
            term *= (seq_t)11;        /* 11*a(i-1) */
            term -= part;             /* subtraction */
            cache[i] = term;
        }
        last_computed = n;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)11;

    ensure_cache(n);
    return cache[n];
}
