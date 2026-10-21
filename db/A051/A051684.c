/*
 * A051684  Auxiliary sequence for calculation of number of even permutations
 *          of degree n and order exactly 2.
 * https://oeis.org/A051684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-1) - (n-1) * (a(n-2) + 1), a(1) = 0, a(2) = -1
 * Range:     i8 n <= 7, i16 n <= 13, i32 n <= 20, i64 n <= 23, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A051684 */
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Memoisation cache: cache[1]..cache[highest] holds computed terms */
    static seq_t *cache   = NULL;
    static idx_t cached   = 0;          /* highest n cached (0 means none) */
    static size_t capacity = 0;         /* allocated size of cache array */
    static bool    init     = false;    /* initialisation flag */

    /* Initialise cache once */
    if (!init) {
        init     = true;
        capacity = 10;                  /* small start; will grow as needed */
        cache    = (seq_t *)malloc(sizeof(seq_t) * capacity);
        if (!cache) abort();            /* allocation failure */

        /* set base values */
        cache[1] = (seq_t)0;            /* a(1) */
        cache[2] = (seq_t)-1;           /* a(2) */
        cached   = 2;
    }

    /* If already computed, return */
    if (n <= cached) return cache[n];

    /* Ensure capacity for index n */
    if ((size_t)n + 2 > capacity) {      /* +2 to keep 1‑based index space */
        size_t newcap = (size_t)n + 2;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * newcap);
        if (!tmp) abort();              /* allocation failure */
        cache    = tmp;
        capacity = newcap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cached + 1; i <= n; ++i) {
        seq_t prev  = cache[i - 1];
        seq_t prev2 = cache[i - 2];
        cache[i] = prev - ((seq_t)(i - 1)) * (prev2 + (seq_t)1);
    }

    cached = n;
    return cache[n];
}
