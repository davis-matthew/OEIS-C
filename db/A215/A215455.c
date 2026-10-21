/*
 * A215455  a(n) = 6*a(n-1) - 9*a(n-2) + a(n-3), with a(0)=3, a(1)=6 and
 *          a(2)=18.
 * https://oeis.org/A215455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 6*a(n-1) - 9*a(n-2) + a(n-3) with a(0)=3, a(1)=6, a(2)=18 */
#include <stddef.h>  /* size_t */
#include <stdlib.h>  /* malloc, free */

/* Return the n-th term of OEIS A215455. */
seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cache = NULL;   /* dynamic array of computed terms      */
    static size_t cap   = 0;      /* current allocation size              */
    static idx_t max_cached = 0;  /* highest index that has been computed */

    /* initialise the cache with the first three known values */
    if (cache == NULL) {
        cap = 4;                     /* small initial capacity */
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)6;
        cache[2] = (seq_t)18;
        max_cached = 2;
    }

    /* If the requested term is already computed, return it. */
    if (n <= max_cached) {
        return cache[(size_t)n];
    }

    /* Grow the cache if necessary to hold at least n+1 terms. */
    size_t needed = (size_t)n + 1;
    if (needed > cap) {
        size_t new_cap = cap * 2;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)malloc(sizeof(seq_t) * new_cap);
        for (size_t i = 0; i < cap; ++i) {
            tmp[i] = cache[i];
        }
        free(cache);
        cache = tmp;
        cap = new_cap;
    }

    /* Compute the missing terms using the recurrence. */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        cache[(size_t)i] =
            (seq_t)6 * cache[(size_t)(i - 1)]
          - (seq_t)9 * cache[(size_t)(i - 2)]
          +             cache[(size_t)(i - 3)];
    }

    max_cached = n;
    return cache[(size_t)n];
}
