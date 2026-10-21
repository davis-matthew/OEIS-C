/*
 * A219788  Consider the succession rule (x, y, z) -> (z, y+z, x+y+z).
 *          Sequence gives z values starting at (0, 1, 2).
 * https://oeis.org/A219788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - a(n-3) Initial terms: a(1)=2, a(2)=3,
 *            a(3)=8
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 27, u64 n <= 55, u128 n <= 110,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of terms stored (== size of cache) */

    /* initialise first three terms if cache empty */
    if (cache == NULL)
    {
        cache_len = 3;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        cache[0] = (seq_t)2;   /* a(1) */
        cache[1] = (seq_t)3;   /* a(2) */
        cache[2] = (seq_t)8;   /* a(3) */
    }

    /* return already computed value */
    if (n <= cache_len)
        return cache[n - 1];

    /* enlarge cache to hold terms up to n */
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * n);
    if (new_cache == NULL) /* allocation failed – return 0 */
        return (seq_t)0;
    cache = new_cache;

    /* compute missing terms using recurrence */
    for (idx_t i = cache_len + 1; i <= n; ++i)
    {
        seq_t val = 2 * cache[i - 2] + cache[i - 3] - cache[i - 4];
        cache[i - 1] = val;
    }

    /* update cache length */
    cache_len = n;
    return cache[n - 1];
}
