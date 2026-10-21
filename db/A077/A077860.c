/*
 * A077860  Expansion of 1/((1 - 2*x + 2*x^2)*(1-x)).
 * https://oeis.org/A077860
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3*a(n-1) - 4*a(n-2) + 2*a(‑3), with a(0) = 1, a(1) = 3,
 *            a(2) = 5
 * Range:     i8 n <= 13, i16 n <= 29, i32 n <= 47, i64 n <= 47, i128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;          /* array of computed terms */
static size_t cache_len = 0;         /* number of initialized terms */
static size_t cache_alloc = 0;       /* size of the allocated array */

/* Return the nth term of A077860. */
seq_t a(idx_t n)
{
    /* Initialise the cache on the first call. */
    if (cache == NULL) {
        cache_alloc = 3;                    /* we know the first 3 values */
        cache = (seq_t *)malloc(cache_alloc * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)5;
        cache_len = 3;
    }

    /* If we already have the term, return it immediately. */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* Grow the cache so that it can accommodate index n. */
    if ((size_t)n >= cache_alloc) {
        size_t new_alloc = cache_alloc > 0 ? cache_alloc : 1;
        while (new_alloc <= (size_t)n)
            new_alloc *= 2;                 /* exponential growth */
        cache = (seq_t *)realloc(cache, new_alloc * sizeof(seq_t));
        cache_alloc = new_alloc;
    }

    /* Compute any terms that are still missing. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        cache[i] = (seq_t)3 * cache[i - 1]
                 - (seq_t)4 * cache[i - 2]
                 + (seq_t)2 * cache[i - 3];
    }
    cache_len = (size_t)n + 1;

    return cache[(size_t)n];
}
