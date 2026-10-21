/*
 * A077973  Expansion of 1/(1+x-2*x^3).
 * https://oeis.org/A077973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-1) + 2*a(n-3); Initial terms: a(0)=1, a(1)=-1,
 *            a(2)=1
 * Range:     i8 n <= 15, i16 n <= 31, i32 n <= 63, i64 n <= 127,
 *            i128 n <= 255, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
/* seq_t and idx_t are defined by the build environment */

static seq_t *cache = NULL;        /* pointer to cached values */
static idx_t cache_capacity = 0;   /* allocated array size (number of elements) */
static idx_t cache_size = 0;       /* number of terms actually computed (cache_size-1 is the last valid index) */

seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (cache_capacity == 0) {
        cache_capacity = 4;          /* allow storage of a(0)..a(3) after first expansion */
        cache_size = 3;              /* we have a(0), a(1), a(2) ready */
        cache = (seq_t *)realloc(NULL, cache_capacity * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)-1;
        cache[2] = (seq_t)1;
    }

    /* early return for the first three terms */
    if (n < 3)
        return cache[n];

    /* If the requested index is beyond what we have computed, extend */
    while (cache_size <= n) {
        /* Ensure enough capacity */
        if (cache_capacity <= n) {
            idx_t new_capacity = cache_capacity;
            if (new_capacity == 0)
                new_capacity = 4;
            while (new_capacity <= n)
                new_capacity <<= 1;   /* double until large enough */
            cache = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
            cache_capacity = new_capacity;
        }

        /* Compute the next term(s) up to n */
        for (idx_t i = cache_size; i <= n; ++i) {
            cache[i] = -cache[i - 1] + (seq_t)2 * cache[i - 3];
        }
        cache_size = n + 1;  /* we now have terms up to a(n) */
    }

    return cache[n];
}
