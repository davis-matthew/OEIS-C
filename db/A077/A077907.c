/*
 * A077907  Expansion of (1-x)^(-1)/(1+x+x^2-2*x^3).
 * https://oeis.org/A077907
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3*a(n-3) - 2*a(n-4) for n >= 4 Initial values: a(0)=1,
 *            a(1)=0, a(2)=0, a(3)=3 (expansion of (1-x)^(-1)/(1+x+x^2-2*x^3))
 * Range:     i8 n <= 12, i16 n <= 25, i32 n <= 48, i64 n <= 97, i128 n <= 196,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Cache for already computed values                     */
    static seq_t *cache = NULL;    /* array of computed terms     */
    static size_t  cap   = 0;      /* allocated array size        */
    static idx_t   max   = 0;      /* highest index computed      */
    static bool    init  = false;  /* cache initialised flag      */

    /* Initialize cache on first call                               */
    if (!init) {
        /* allocate a small initial array                         */
        cap = 8;
        cache = (seq_t *)realloc(NULL, cap * sizeof(seq_t));
        if (!cache) exit(1);

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)3;

        max = 3;
        init = true;
    }

    /* Grow the cache if necessary to hold up to index n */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap;
        while ((size_t)n + 1 > new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cap = new_cap;
    }

    /* Compute any terms that are not yet known */
    for (idx_t i = max + 1; i <= n; ++i) {
        if (i <= 3) {            /* already initialised */
            continue;
        }
        /* recurrence: a[i] = 3*a[i-3] - 2*a[i-4] */
        seq_t val3 = cache[i - 3] * (seq_t)3;
        seq_t val2 = cache[i - 4] * (seq_t)2;
        cache[i] = val3 - val2;
    }
    max = (max > n) ? max : n;

    return cache[n];
}
