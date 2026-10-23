/*
 * A049859  a(n) = Sum_{k=0,1,2,...,n-4,n-2,n-1} a(k); a(n-3) is not a
 *          summand, with a(0)=0, a(1)=1, a(2)=3.
 * https://oeis.org/A049859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-3) + a(n-4) for n >= 4 with a(0)=0,
 *            a(1)=1, a(2)=3, a(3)=4
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 36, u64 n <= 72, u128 n <= 143,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of initialized entries */
    static idx_t cache_cap = 0;   /* allocated capacity */

    /* minimal capacity needed to hold first four terms */
    idx_t min_cap = 4;
    if (n + 1 > min_cap)
        min_cap = n + 1;

    /* initialization */
    if (!cache)
    {
        cache_cap = min_cap;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;              /* allocate failure */

        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)4;
        cache_len = 4;                     /* indices 0..3 initialized */
    }
    else if (n + 1 > cache_cap)
    {
        /* grow cache to at least n+1 */
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                /* allocate failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    if (n < cache_len)
        return cache[n];

    /* compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        /* recurrence: a(i) = 2*a(i-1) - a(i-3) + a(i-4) */
        cache[i] = 2 * cache[i - 1] - cache[i - 3] + cache[i - 4];
    }
    cache_len = n + 1;
    return cache[n];
}
