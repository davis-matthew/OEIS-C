/*
 * A112523  Expansion of x*(1+3*x-4*x^2-5*x^3-4*x^6+4*x^5+3*x^4) /
 *          ((1+4*x^2)*(1+x^2)*(1-x^2+x^4)).
 * https://oeis.org/A112523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -4*a(n-2) - a(n-6) - 4*a(n-8) for n ≥ 8, * with initial
 *            values: * a(0)= 0, a(1)= 1, a(2)= 3, a(3)=-8, * a(4)=-17,
 *            a(5)=35, a(6)=72, a(7)=-145 * * The function memoises the
 *            sequence in a static dynamic array, * so that repeated calls (in
 *            any order) are fast
 * Range:     i8 n <= 6, i16 n <= 14, i32 n <= 30, i64 n <= 62, i128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;   /* array of values */
    static idx_t cache_size = 0;  /* number of values currently stored */
    static idx_t cache_cap   = 0;  /* allocated capacity */

    /* Create cache if it does not exist yet */
    if (cache == NULL) {
        /* initial capacity sufficient for the first 8 values */
        cache_cap = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (cache == NULL) return 0;        /* allocation failure (should not happen) */

        /* initialise the first 8 terms */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)-8;
        cache[4] = (seq_t)-17;
        cache[5] = (seq_t)35;
        cache[6] = (seq_t)72;
        cache[7] = (seq_t)-145;
        cache_size = 8;
    }

    /* Grow the cache if required */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap * 2;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (new_cache == NULL) return 0;     /* allocation failure */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* If we already have the value, return it */
    if (n < cache_size)
        return cache[(size_t)n];

    /* Compute missing values up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* compute a(i) using the recurrence */
        /* all indices here are guaranteed to be < i, so already computed */
        seq_t val = -(seq_t)4 * cache[(size_t)(i - 2)]
                    - cache[(size_t)(i - 6)]
                    - (seq_t)4 * cache[(size_t)(i - 8)];
        cache[(size_t)i] = val;
    }
    cache_size = n + 1;
    return cache[(size_t)n];
}
