/*
 * A045873  a(n) = A006496(n)/2.
 * https://oeis.org/A045873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*a(n-1) - 5*a(n-2), a(0)=0, a(1)=1;
 * Range:     i8 n <= 6, i16 n <= 14, i32 n <= 28, i64 n <= 55, i128 n <= 111,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* sequence a(n) (OEIS A045873) */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;

    /* initialise cache for the first two terms */
    if (cache == NULL) {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();              /* out of memory */
        cache[0] = 0;                     /* a(0) */
        cache[1] = 1;                     /* a(1) */
    }

    /* extend cache up to requested index */
    if (n >= cache_cap) {
        idx_t new_cap = n + 1;            /* need indices 0..n inclusive */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                /* out of memory */
        cache = tmp;
        /* compute new terms using the recurrence */
        for (idx_t i = cache_cap; i <= n; ++i) {
            cache[i] = ((seq_t)2 * cache[i-1]) - ((seq_t)5 * cache[i-2]);
        }
        cache_cap = new_cap;
    }

    return cache[n];
}
