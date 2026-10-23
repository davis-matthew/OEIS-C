/*
 * A109781  Expansion of (-1+x+2*x^2-6*x^3+x^4+x^5) /
 *          ((x-1)*(x^2-x+1)*(x^2-2*x-1)*(x+1)^2).
 * https://oeis.org/A109781
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -2*a(n-1) + 2*a(n-2) + a(n-3) - 3*a(n-4) + 2*a(n-5) +
 *            2*a(n-6) - a(n-7) for n>6 Initial values: a(0) = -1, a(1) = 3,
 *            a(2) = -6, a(3) = 11, a(4) = -27, a(5) = 60, a(6) = -141
 * Range:     i8 n <= 5, i16 n <= 12, i32 n <= 24, i64 n <= 49, i128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* first 7 terms of the sequence */
    static const seq_t init[7] = { -1, 3, -6, 11, -27, 60, -141 };

    /* dynamic cache to store computed terms */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;  /* allocated space (in terms) */
    static idx_t sz   = 0;   /* number of terms actually stored */

    /* for small indices just return the known value */
    if (n < 7) {
        return init[n];
    }

    /* initialise cache on first use */
    if (!cache) {
        cap = 8;  /* start with a capacity of 8 terms */
        cache = (seq_t *)malloc((size_t)cap * sizeof(seq_t));
        for (idx_t i = 0; i < 7; ++i) {
            cache[i] = init[i];
        }
        sz = 7;   /* first 7 terms are now cached */
    }

    /* grow cache if requested index is beyond current size */
    if (n >= sz) {
        /* increase allocation size geometrically */
        idx_t new_cap = cap;
        while (new_cap <= n) {
            new_cap <<= 1;
        }
        if (new_cap != cap) {
            cache = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
            cap = new_cap;
        }

        /* compute all missing terms up to n inclusively */
        for (idx_t i = sz; i <= n; ++i) {
            /* recurrence using only seq_t arithmetic (no intermediate truncation) */
            seq_t term = ((seq_t)-2) * cache[i-1]
                       + ((seq_t) 2) * cache[i-2]
                       + ((seq_t) 1) * cache[i-3]
                       + ((seq_t)-3) * cache[i-4]
                       + ((seq_t) 2) * cache[i-5]
                       + ((seq_t) 2) * cache[i-6]
                       + ((seq_t)-1) * cache[i-7];
            cache[i] = term;
        }
        sz = n + 1;
    }

    return cache[n];
}
