/*
 * A167617  G.f.: x^2*(3+3*x+x^2) / ( (2*x+1) * (1+x) * (1+x+x^2) * (x^2-x+1)
 *          ) .
 * https://oeis.org/A167617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 31, i64 n <= 63, i128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = -3*a(n-1)-3*a(n-2)-3*a(n-3)-3*a(n-4)-3*a(n-5)-2*a(n-6);
   Initial values: a(0)=0, a(1)=0, a(2)=3, a(3)=-6, a(4)=10, a(5)=-21. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t len  = 0;   /* number of elements stored (highest index + 1) */
    static idx_t cap  = 0;   /* capacity of the allocated array */

    /* Initial allocation and fill of the first 6 terms */
    if (len == 0) {
        cap   = 16;                        /* initial capacity */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);

        static const seq_t init[6] = {0, 0, 3, -6, 10, -21};
        for (idx_t i = 0; i < 6; ++i)
            cache[i] = init[i];
        len = 6;
    }

    /* Grow the cache if we need to compute further terms */
    if (n >= len) {
        idx_t new_cap = cap;
        while (new_cap <= n)
            new_cap <<= 1;                       /* double until enough */

        if (new_cap > cap) {
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!cache) exit(EXIT_FAILURE);
            cap = new_cap;
        }

        /* Compute terms from the first missing one up to n */
        for (idx_t i = len; i <= n; ++i) {
            cache[i] = -3*cache[i-1] - 3*cache[i-2]
                     - 3*cache[i-3] - 3*cache[i-4]
                     - 3*cache[i-5] - 2*cache[i-6];
        }
        len = n + 1;
    }

    return cache[n];
}
