/*
 * A233951  Number of (n+1) X (1+1) 0..5 arrays with every 2 X 2 subblock
 *          having the sum of the squares of the edge differences equal to 10,
 *          and no two adjacent values equal.
 * https://oeis.org/A233951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 5*a(n-2) – 7*a(n-3) – 6*a(n-4) (n ≥ 5) Initial
 *            values: a(1)=80, a(2)=380, a(3)=1776, a(4)=8336
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined for indices below the offset */

    /* Static cache: cache[i] holds a(i) for 1 ≤ i ≤ cache_last. */
    static seq_t *cache = NULL;
    static idx_t cache_last = 0;   /* highest index currently stored */
    static idx_t cache_cap = 0;    /* allocated size (highest available index) */

    /* Grow the cache if needed. We allocate an extra element for a(1) at index 1. */
    if (n > cache_cap)
    {
        idx_t new_cap = n;          /* capacity needed (index n exists) */
        seq_t *new_cache = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (new_cache == NULL)
            abort();                /* allocation failure */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = cache_last + 1; i <= n; ++i)
    {
        if (i <= 4)          /* initial values */
        {
            switch (i)
            {
                case 1: cache[i] = (seq_t)80; break;
                case 2: cache[i] = (seq_t)380; break;
                case 3: cache[i] = (seq_t)1776; break;
                case 4: cache[i] = (seq_t)8336; break;
            }
        }
        else                 /* recurrence: a(n) = 4*a(n-1)+5*a(n-2)-7*a(n-3)-6*a(n-4) */
        {
            seq_t val = 4 * cache[i - 1] + 5 * cache[i - 2];
            val -= 7 * cache[i - 3];
            val -= 6 * cache[i - 4];
            cache[i] = val;
        }
    }

    cache_last = n;
    return cache[n];
}
