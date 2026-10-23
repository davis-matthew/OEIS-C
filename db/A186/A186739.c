/*
 * A186739  a(0)=0, a(1)=0; for n>1, a(n) = a(n-1) + (n-2)*a(n-2) + 1.
 * https://oeis.org/A186739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(1) = 0; for n > 1, a(n) = a(n-1) + (n-2) * a(n-2) +
 *            1
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 19, u64 n <= 32, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cache     = NULL;   /* array of coefficients */
    static size_t  cache_cap = 0;     /* allocated size of cache   */
    static idx_t   cached_max = 0;    /* highest index that is valid */

    /* initialise cache for n = 0 and n = 1 */
    if (cache == NULL)
    {
        /* we store a(0) and a(1) */
        cache     = (seq_t *)malloc(2 * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;          /* allocation failure */
        cache[0]  = (seq_t)0;
        cache[1]  = (seq_t)0;
        cache_cap = 2;
        cached_max = 1;   /* we already have values for 0 and 1   */
    }

    /* if we already have the requested value, return it immediately */
    if (n <= cached_max)
        return cache[n];

    /* make sure the cache is large enough for index n */
    if ((size_t)(n + 1) > cache_cap)
    {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;          /* allocation failure */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* compute all missing terms up to n */
    for (idx_t i = cached_max + 1; i <= n; ++i)
    {
        /* recurrence uses a(i-1) and a(i-2) – both already present in cache */
        seq_t term = cache[i - 1] + ((seq_t)(i - 2)) * cache[i - 2] + (seq_t)1;
        cache[i] = term;
    }
    cached_max = n;

    return cache[n];
}
