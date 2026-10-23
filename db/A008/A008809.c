/*
 * A008809  Expansion of (1+x^9)/((1-x^2)^2*(1-x^9)).
 * https://oeis.org/A008809
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-2) - a(n-4) + a(n-9) - 2*a(n-11) + a(n-13) + [n==0
 *            or n==9], with a(k)=0 for k<0. This recurrence comes from the
 *            generating function (1+x^9)/((1-x^2)^2*(1-x^9))
 * Range:     u8 n <= 93, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static cache of previously computed values. */
    static seq_t *cache = NULL;      /* array of a(i) for i <= maxComputed */
    static size_t cap = 0;           /* current capacity of the cache */
    static idx_t maxComputed = 0;    /* largest index for which cache[i] is valid */

    /* Initialise cache on first invocation. */
    if (cache == NULL) {
        cap = 16;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)1;        /* a(0) = 1 */
        maxComputed = 0;
    }

    /* Increase cache size if required. */
    if ((idx_t)cap <= n) {
        size_t newCap = cap;
        while ((idx_t)newCap <= n) newCap <<= 1;   /* double until sufficient */
        seq_t *tmp = (seq_t*)realloc(cache, newCap * sizeof(seq_t));
        cache = tmp;
        cap = newCap;
    }

    /* Compute missing entries up to n. */
    if (n > maxComputed) {
        for (idx_t i = maxComputed + 1; i <= n; ++i) {
            size_t ii = (size_t)i;
            seq_t val = 0;

            /* Positive terms. */
            if (i >= 2)      val += cache[ii - 2] * (seq_t)2;
            if (i >= 9)      val += cache[ii - 9];
            if (i >= 13)     val += cache[ii - 13];
            if (i == 0 || i == 9) val += (seq_t)1;  /* RHS of recurrence */

            /* Negative terms. */
            if (i >= 4)      val -= cache[ii - 4];
            if (i >= 11)     val -= cache[ii - 11] * (seq_t)2;

            cache[ii] = val;
        }
        maxComputed = n;
    }

    return cache[(size_t)n];
}
