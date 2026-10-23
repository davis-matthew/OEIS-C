/*
 * A192875  Coefficient of x in the reduction by (x^2 -> x + 1) of the
 *          polynomial p(n,x) given in Comments.
 * https://oeis.org/A192875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 6*a(n-2) - 5*a(n-3) - 6*a(n-4) + 4*a(n-5) with
 *            a(0)=0, a(1)=1, a(2)=3, a(3)=11, a(4)=37, a(5)=119
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* initialize cache on first call */
    if (cache == NULL) {
        cache_cap = 16;                      /* small initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;        /* allocation failure (rare) */
        /* predefined first six terms */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)11;
        cache[4] = (seq_t)37;
        cache[5] = (seq_t)119;
        cache_len = 6;
    }

    /* if requested index is already cached, return it */
    if (n < cache_len)
        return cache[(size_t)n];

    /* ensure enough capacity */
    size_t required = (size_t)n + 1;
    if (required > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < required)
            new_cap <<= 1;          /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms using the recurrence */
    size_t i;
    for (i = cache_len; i <= (size_t)n; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];
        seq_t a5 = cache[i - 5];
        cache[i] = ((seq_t)2 * a1) + ((seq_t)6 * a2)
                 - ((seq_t)5 * a3) - ((seq_t)6 * a4)
                 + ((seq_t)4 * a5);
    }
    cache_len = required;

    return cache[(size_t)n];
}
