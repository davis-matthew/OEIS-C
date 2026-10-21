/*
 * A224749  Vauban's sequence: a(n)=0 if n<=0, a(1)=1; thereafter a(n) =
 *          3*a(n-1) + 6*a(n-2) + 6*a(n-3) + 6*a(n-4) + 6*a(n-5).
 * https://oeis.org/A224749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 6*a(n-2) + 6*a(n-3) + 6*a(n-4) + 6*a(n-5) with
 *            a(0)=0, a(1)=1, a(2)=3, a(3)=15, a(4)=69, a(5)=321
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 30, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache and bookkeeping */
    static seq_t *cache = 0;       /* pointer to cached values */
    static idx_t last_n = 0;       /* largest n computed so far */
    static size_t cache_cap = 0;   /* capacity of the cache (in elements) */

    /* initialise cache on first call */
    if (cache == 0)
    {
        cache_cap = 6;                                 /* indices 0 … 5 inclusive */
        cache     = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;                           /* allocation failed */

        /* base values given in the OEIS entry */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)15;
        cache[4] = (seq_t)69;
        cache[5] = (seq_t)321;
        last_n = 5;
    }

    /* if the requested index is already cached, return it */
    if (n <= last_n)
        return cache[n];

    /* grow cache if necessary */
    if ((size_t)n + 1 > cache_cap)
    {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;                           /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute all missing terms up to n */
    for (idx_t i = last_n + 1; i <= n; ++i)
    {
        cache[i] =
            (seq_t)3 * cache[i - 1] +
            (seq_t)6 * cache[i - 2] +
            (seq_t)6 * cache[i - 3] +
            (seq_t)6 * cache[i - 4] +
            (seq_t)6 * cache[i - 5];
    }

    last_n = n;
    return cache[n];
}
