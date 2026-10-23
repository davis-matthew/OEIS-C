/*
 * A186335  A transform of the central binomial coefficients.
 * https://oeis.org/A186335
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) follows the 5-term recurrence * n*a(n) + (-2*n+1)*a(n-1) +
 *            5*(-n+1)*a(n-2) * +3*(2*n-3)*a(n-3) +5*(n-2)*a(n-4) = 0 *
 *            Initial values: a(0)=1, a(1)=1, a(2)=4, a(3)=7 * Recurrence
 *            rearranged as: * a(n) = ((2*n-1)*a(n-1) + 5*(n-1)*a(n-2) *
 *            -3*(2*n-3)*a(n-3) -5*(n-2)*a(n-4)) / n
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include <stdbool.h>  /* for bool */
#include <stdlib.h>   /* for malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;    /* allocated capacity */
    static idx_t   last   = 0;    /* highest index computed */
    static bool    init   = false;

    /* initialise base values on first use */
    if (!init) {
        /* allocate space for indices 0..3 */
        cap = 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;           /* defensive: allocation failed */
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 4;
        cache[3] = 7;
        last    = 3;
        init    = true;
    }

    /* if the requested index is already computed, just return it */
    if (n <= last)
        return cache[n];

    /* ensure enough space for all requested indices */
    if (n >= cap) {
        size_t newcap = cap == 0 ? 4 : cap;
        while (newcap <= (size_t)n)
            newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0;                   /* defensive: allocation failed */
        cache = tmp;
        cap   = newcap;
    }

    /* compute newly required terms using the recurrence */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* all arithmetic on seq_t; constants cast to seq_t first */
        seq_t term1 = (seq_t)(2 * i - 1) * cache[i - 1];
        seq_t term2 = (seq_t)5 * cache[i - 2] * (seq_t)(i - 1);
        seq_t term3 = (seq_t)3 * cache[i - 3] * (seq_t)(2 * i - 3);
        seq_t term4 = (seq_t)5 * cache[i - 4] * (seq_t)(i - 2);

        seq_t numer = term1 + term2;
        numer -= term3;
        numer -= term4;

        cache[i] = numer / (seq_t)i;   /* division is exact by construction */
    }
    last = n;
    return cache[n];
}
