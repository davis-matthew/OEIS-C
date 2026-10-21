/*
 * A071440  Start with 1; add the digits of the previous term and the squares
 *          of the digits of the previous term.
 * https://oeis.org/A071440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0)=1; for n>0, a(n)=Σ_{digits d of a(n-1)} (d + d*d)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the recursive definition of OEIS sequence A071440. */

#include <stdlib.h>

/* The build supplies the types seq_t and idx_t. */

seq_t a(idx_t n)
{
    /* Static cache of computed terms.  cache[0] holds a(0)=1. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of elements stored */
    static size_t cache_cap = 0;  /* capacity of allocated array */

    /* Initialise cache on first call. */
    if (cache == NULL) {
        cache_cap = 16;                     /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL)                 /* out of memory */
            return (seq_t)0;                /* fallback (no error handling) */
        cache[0] = (seq_t)1;                /* a(0) */
        cache_len = 1;
    }

    /* Extend the cache up to index n (if necessary). */
    if (n >= cache_len) {
        /* Ensure sufficient capacity. */
        if ((size_t)n + 1 > cache_cap) {
            size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
            while ((size_t)(n + 1) > new_cap)
                new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL)                 /* allocation failed */
                return (seq_t)0;             /* fallback */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute successive terms. */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            seq_t temp = prev;
            seq_t sum = 0;

            /* Sum over all decimal digits: d + d^2. */
            while (temp > 0) {
                unsigned d = (unsigned)(temp % 10);  /* digit 0-9 */
                sum += (seq_t)d + (seq_t)d * (seq_t)d;
                temp /= 10;
            }

            cache[i] = sum;
        }
        cache_len = n + 1;
    }

    return cache[(size_t)n];
}
