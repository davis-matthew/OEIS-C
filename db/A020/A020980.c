/*
 * A020980  Expansion of 1/((1-8*x)*(1-10*x)*(1-12*x)).
 * https://oeis.org/A020980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 30*a(n-1) - 296*a(n-2) + 960*a(n-3), a(0)=1, a(1)=30,
 *            a(2)=604
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;       /* memoization array */
    static idx_t cache_size = 0;       /* allocated size of cache   */
    static idx_t last_computed = 0;    /* highest index with a known value */

    /* Ensure the cache is large enough for index n */
    idx_t needed = n + 1;
    if (cache_size < needed)
    {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp && needed > 0)        /* allocation failure   */
            abort();
        cache = tmp;
        cache_size = needed;
    }

    /* Initialise the first three terms if they are not yet known */
    if (last_computed < 2)
    {
        /* Make sure we have room for indices 0,1,2 */
        if (cache_size < 3)
        {
            seq_t *tmp = (seq_t *)realloc(cache, 3 * sizeof(seq_t));
            if (!tmp && 3 > 0) abort();
            cache = tmp;
            cache_size = 3;
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)30;
        cache[2] = (seq_t)604;
        last_computed = 2;
    }

    /* Value already computed? */
    if (n <= last_computed)
        return cache[n];

    /* Compute missing terms using the recurrence until n */
    for (idx_t i = last_computed + 1; i <= n; ++i)
    {
        seq_t v = 30 * cache[i - 1] - 296 * cache[i - 2] + 960 * cache[i - 3];
        cache[i] = v;
    }
    last_computed = n;
    return cache[n];
}
