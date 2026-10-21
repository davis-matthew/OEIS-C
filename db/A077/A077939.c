/*
 * A077939  Expansion of 1/(1 - 2*x - x^2 - x^3).
 * https://oeis.org/A077939
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + a(n-3); with initial values a(0) = 1,
 *            a(1) = 2, a(2) = 5
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 47, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses a static cache so that calls in any order
   remain O(n) in total over all queries.  The cache is grown
   with realloc, which is available in both C11 and C++17. */
#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc, free */

/* The external types seq_t and idx_t are provided by the build
   environment.  seq_t is an unsigned integer type of arbitrary size. */

seq_t a(idx_t n)
{
    /* Static state – a simple dynamic array of already computed terms. */
    static seq_t *cache = NULL;    /* pointer to the cache array */
    static size_t capacity = 0;    /* current allocated capacity */
    static idx_t last_computed = (idx_t)(-1);  /* highest index computed */

    /* ---- First use: create the cache with the three initial terms ---- */
    if (cache == NULL) {
        capacity = 3;                       /* at least three slots */
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (cache == NULL) {                 /* allocation failure – abort */
            /* In a real‑world library we might throw an exception
               or return an error code; for the purpose of this
               problem we simply exit. */
            exit(1);
        }
        cache[0] = 1;        /* a(0) */
        cache[1] = 2;        /* a(1) */
        cache[2] = 5;        /* a(2) */
        last_computed = 2;
    }

    /* ---- Ensure the array is large enough for n ---- */
    /* Convert n to size_t for the comparison; n is unsigned, so no overflow
       occurs here for the test values of interest. */
    size_t required = (size_t)n + 1;
    if (required > capacity) {
        /* Grow the cache exactly to the required size – a small
           upgrade strategy (e.g. double) is unnecessary for the
           expected n ranges. */
        seq_t *tmp = (seq_t *)realloc(cache, required * sizeof(seq_t));
        if (tmp == NULL) { /* allocation failure – abort */
            exit(1);
        }
        cache = tmp;
        capacity = required;
    }

    /* ---- Compute any missing terms up to n ---- */
    if ((idx_t)n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            seq_t val = cache[i - 1];           /* 2*a(i-1) – first add */
            val += cache[i - 1];                /* second add (total 2*a(i-1)) */
            val += cache[i - 2];                /* + a(i-2) */
            val += cache[i - 3];                /* + a(i-3) */
            cache[i] = val;
        }
        last_computed = n;
    }

    return cache[n];
}
