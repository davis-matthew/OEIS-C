/*
 * A078066  Expansion of (1-x)/(1+2*x+x^2+2*x^3).
 * https://oeis.org/A078066
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -2*a(n-1) - a(n-2) - 2*a(n-3) Initial conditions: a(0) =
 *            1, a(1) = -3, a(2) = 5
 * Range:     i8 n <= 6, i16 n <= 14, i32 n <= 30, i64 n <= 33, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for the sequence values */
    static seq_t *cache = NULL;   /* array of computed terms           */
    static size_t  cap   = 0;     /* capacity of cache                 */
    static idx_t   max_n = 0;     /* largest n that has been computed   */

    /* first-time initialization */
    if (cache == NULL)
    {
        /* start with a minimal capacity */
        size_t init_cap = sizeof(seq_t) * 10;
        cache = (seq_t *)malloc(init_cap);
        if (cache == NULL) abort();

        cap   = init_cap / sizeof(seq_t);
        max_n = 2;  /* we will set a[0..2] */

        /* seed initial values */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)(-3);
        cache[2] = (seq_t)5;
    }

    /* if the requested n is already known, return it */
    if (n <= max_n)
        return cache[n];

    /* enlarge the cache if necessary */
    if ((size_t)n + 1 > cap)
    {
        size_t new_cap = (size_t)(n + 1);
        if (new_cap < 10) new_cap = 10;
        if (new_cap < cap * 2) new_cap = cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = max_n + 1; i <= n; ++i)
    {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];

        /* use an intermediate wide type to avoid overflow issues
           (__int128 is a GNU extension; the compiler should support it) */
        __int128 val = -(__int128)a1 - (__int128)a1
                     - (__int128)a2
                     - (__int128)a3 - (__int128)a3;
        cache[i] = (seq_t)val;
    }

    max_n = n;
    return cache[n];
}
