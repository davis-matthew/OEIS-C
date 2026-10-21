/*
 * A080096  a(1) = a(2) = 1, a(3) = 2, thereafter a(n) = abs(a(n-1) - a(n-2) -
 *          a(n-3)).
 * https://oeis.org/A080096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |a(n-1) - a(n-2) - a(n-3)|, a(1)=a(2)=1, a(3)=2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


/* absolute difference of the last three terms */
static inline seq_t abs_diff(seq_t x, seq_t y, seq_t z)
{
    seq_t sum = y + z;
    return (x >= sum) ? (x - sum) : (sum - x);
}

seq_t a(idx_t n)
{
    /* static cache indexed from 1 */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;
    static idx_t len   = 0;

    /* initialise cache on first call */
    if (cache == NULL)
    {
        cap = 4;                                 /* hold indices 1..3 + spare */
        cache = (seq_t*)malloc((cap + 1) * sizeof(seq_t));
        if (!cache) return (seq_t)0;             /* allocation failed */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)2;
        len = 3;
    }

    /* grow capacity if needed */
    if (n > cap)
    {
        while (cap < n) cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, (cap + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;              /* allocation failed */
        cache = tmp;
    }

    /* compute missing terms */
    if (n > len)
    {
        for (idx_t i = len + 1; i <= n; ++i)
            cache[i] = abs_diff(cache[i-1], cache[i-2], cache[i-3]);

        len = n;
    }

    return cache[n];
}
