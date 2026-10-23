/*
 * A016190  Expansion of 1/((1-9*x)*(1-11*x)).
 * https://oeis.org/A016190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A016190
 *  Expansion of 1/((1-9*x)*(1-11*x)).
 *
 *  Formula (closed form):
 *      a(n) = (11^(n+1) - 9^(n+1)) / 2
 *
 *  Recurrence for computation:
 *      a(0) = 1
 *      a(n) = 11 * a(n-1) + 9^n      (n >= 1)
 */

#include <stdlib.h>   /* malloc, realloc, NULL */
#include <stddef.h>   /* size_t */

/* Compute a(n) using the recurrence and cache results for arbitrary order of calls. */
seq_t a(idx_t n)
{
    /* Static cache state: cached values, current capacity, highest index computed, and current 9^i. */
    static seq_t *cache      = NULL;
    static size_t  cap       = 0;
    static idx_t   last     = 0;
    static seq_t   pow9      = 0;   /* 9^i for i = last */

    /* --- Initialisation on first call ------------------------------------ */
    if (cache == NULL) {
        /* need space for a(0) only */
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;          /* out of memory */
        cap       = 1;
        cache[0]  = (seq_t)1;        /* a(0) = 1 */
        pow9      = (seq_t)1;        /* 9^0 = 1 */
        last      = 0;
    }

    /* If requested index is already cached, return it. */
    if (n <= last) {
        return cache[n];
    }

    /* Enlarge cache if necessary. */
    if (n + 1 > cap) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;          /* out of memory */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute successive terms up to n. */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* 9^i from 9^(i-1).  multiplication by small constant is safe for all seq_t. */
        pow9 = pow9 * (seq_t)9;

        /* a(i) = 11 * a(i-1) + 9^i. */
        seq_t term = cache[i-1] * (seq_t)11;
        cache[i]   = term + pow9;
    }

    last = n;
    return cache[n];
}
