/*
 * A159521  Numerator of Hermite(n, 1/16).
 * https://oeis.org/A159521
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0) = 1, a(1) = 1, a(n) = a(n-1) - 128*(n-1)*a(n-2) (for n >=
 *            2)
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 7, i64 n <= 13, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This follows from the Hermite recurrence at x = 1/16. */

#include <stdlib.h>
#include <stdint.h>

/* External linkage as required */
seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;

    /* Initialise cache with first two terms */
    if (cache == NULL) {
        cache_cap = 4;                     /* start with a small capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) {
            /* allocation failure – terminate the program */
            abort();
        }
        cache[0] = (seq_t)1;               /* a(0) */
        cache[1] = (seq_t)1;               /* a(1) */
        cache_len = 2;
    }

    /* If we have already computed up to n, return it */
    if (n < cache_len)
        return cache[n];

    /* Grow cache if necessary */
    while (cache_cap <= n + 1) {
        size_t new_cap = (cache_cap == 0) ? 4 : (cache_cap << 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms iteratively from cache_len up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0 || i == 1) {
            /* Base cases already initialised */
            continue;
        }
        /* Recurrence: a(i) = a(i-1) - 128*(i-1)*a(i-2) */
        idx_t m = i - 1;
        seq_t coeff = ((seq_t)128 * (seq_t)m);    /* 128*(i-1) as seq_t */
        seq_t prod = coeff * cache[i - 2];
        cache[i] = cache[i - 1] - prod;
    }

    cache_len = n + 1;
    return cache[n];
}
