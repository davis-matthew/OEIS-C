/*
 * A229374  Number of n X 2 0..2 arrays avoiding 11 horizontally, 22
 *          vertically and 00 diagonally or antidiagonally.
 * https://oeis.org/A229374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + a(n-2) - 2*a(n-3), a(1)=8, a(2)=30, a(3)=126
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* n must be >= 1; behaviour for n<1 is undefined */
    if (n == 0)
        return (seq_t)0;

    /* Static cache of computed values */
    static seq_t *cache = NULL;   /* pointer to allocated array */
    static idx_t  size = 0;      /* number of terms actually computed */
    static idx_t  cap  = 0;      /* capacity of the allocated array */

    /* Ensure at least the first three terms are available */
    if (cache == NULL) {
        cap = (n > 3 ? n : 3);                   /* initial capacity */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (cache == NULL)                       /* allocation failure */
            return (seq_t)0;
        cache[0] = (seq_t)8;                     /* a(1) */
        cache[1] = (seq_t)30;                    /* a(2) */
        cache[2] = (seq_t)126;                   /* a(3) */
        size = 3;                                /* three terms computed */
    }

    /* Grow capacity if required */
    if (cap < n) {
        idx_t new_cap = cap * 2;                 /* exponential growth */
        if (new_cap < n)
            new_cap = n;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)                         /* reallocation failure */
            return (seq_t)0;
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing terms up to n */
    while (size < n) {
        /* indices in cache: 0-based, refer to a(1) at cache[0] */
        seq_t val = (seq_t)4 * cache[size - 1]
                    + cache[size - 2]
                    - (seq_t)2 * cache[size - 3];
        cache[size++] = val;
    }

    return cache[n - 1];
}
