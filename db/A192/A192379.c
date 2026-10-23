/*
 * A192379  Constant term of the reduction by x^2->x+1 of the polynomial
 *          p(n,x) defined below in Comments.
 * https://oeis.org/A192379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 6*a(n-2) - 2*a(n-3) - a(n-4) (n >= 5) Initial
 *            values: a(1) = 1, a(2) = 0, a(3) = 5, a(4) = 8
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoisation cache holds the already computed terms. */
    static seq_t *cache = NULL;              /* array of values, index 0 unused   */
    static idx_t cache_size = 0;             /* largest index that has been computed */
    static idx_t cache_capacity = 0;         /* allocated size of cache array     */

    /* Handle the first call: initialise the cache and the first four terms. */
    if (cache == NULL)
    {
        cache_capacity = 8;                                 /* initial capacity            */
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        if (!cache) exit(1);

        cache[0] = (seq_t)0;               /* dummy for 0‑based indexing */
        cache[1] = (seq_t)1;               /* a(1) */
        cache[2] = (seq_t)0;               /* a(2) */
        cache[3] = (seq_t)5;               /* a(3) */
        cache[4] = (seq_t)8;               /* a(4) */
        cache_size = 4;                    /* highest computed index       */
    }

    /* Grow the backing array if the requested index is larger than current capacity. */
    if (n > cache_capacity - 1)
    {
        /* Double the capacity until it is enough. This keeps the number of reallocs
           logarithmic in the maximum requested index. */
        idx_t new_cap = cache_capacity * 2;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_capacity = new_cap;
    }

    /* Compute any missing terms up to the requested index. */
    if (n > cache_size)
    {
        for (idx_t i = cache_size + 1; i <= n; ++i)
        {
            seq_t t1 = cache[i - 1];
            seq_t t2 = cache[i - 2];
            seq_t t3 = cache[i - 3];
            seq_t t4 = cache[i - 4];
            seq_t tmp = (seq_t)2 * t1 + (seq_t)6 * t2;   /* 2*a(i-1)+6*a(i-2)       */
            tmp -= (seq_t)2 * t3;                       /* subtract 2*a(i-3)        */
            tmp -= t4;                                  /* subtract a(i-4)          */
            cache[i] = tmp;
        }
        cache_size = n;
    }

    return cache[n];
}
