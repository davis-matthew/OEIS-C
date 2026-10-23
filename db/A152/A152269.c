/*
 * A152269  Expansion of g.f. x*(1-3*x-8*x^2)/(1+5*x^2-32*x^4).
 * https://oeis.org/A152269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n) = -5*a(n-2) + 32*a(n-4) with a(0)=0, a(1)=1, a(2)=-3,
 *            a(3)=-13
 * Range:     i8 n <= 5, i16 n <= 10, i32 n <= 20, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* initial values for n < 4 */
    if (n < 4) {
        static const seq_t init[4] = { 0, 1, -3, -13 };
        return init[(size_t)n];
    }

    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cap   = 0;        /* number of slots allocated */
    static size_t last = 3;        /* highest computed index */

    if (cache == NULL) {
        /* allocate cache for at least the first four values */
        cap   = 8;          /* start with a small size, we will grow as needed */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = 0; cache[1] = 1; cache[2] = -3; cache[3] = -13;
        last = 3;
    }

    /* grow cache if necessary */
    while ((size_t)n >= cap) {
        size_t new_cap = cap * 2;
        if (new_cap <= cap) new_cap = cap + 1;   /* safety for overflow */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing values up to n */
    for (size_t i = last + 1; i <= (size_t)n; ++i) {
        cache[i] = (-5) * cache[i - 2] + 32 * cache[i - 4];
    }
    last = (size_t)n;

    return cache[(size_t)n];
}
