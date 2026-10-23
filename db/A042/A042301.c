/*
 * A042301  Denominators of continued fraction convergents to sqrt(677).
 * https://oeis.org/A042301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 52*a(n-1) + a(n-2); a(0) = 1, a(1) = 52
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
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
    static idx_t cached_size = 0; /* number of elements stored (i.e., last index + 1) */
    static idx_t cached_cap   = 0; /* capacity of cache */

    /* initialise cache if this is the first call */
    if (cache == NULL)
    {
        /* start with a modest capacity and allocate */
        cached_cap = 4;
        cache = (seq_t *)malloc(cached_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failed – unlikely in tests */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)52;
        cached_size = 2;
    }

    /* extend cache to contain index n if necessary */
    if (n >= cached_size)
    {
        /* grow capacity to at least n+1 */
        while (cached_cap <= n)
            cached_cap <<= 1;           /* double the capacity */

        seq_t *tmp = (seq_t *)realloc(cache, cached_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;     /* allocation failed – unlikely */
        cache = tmp;

        /* compute missing terms using the recurrence */
        for (idx_t i = cached_size; i <= n; ++i)
        {
            cache[i] = cache[i - 1] * (seq_t)52 + cache[i - 2];
        }
        cached_size = n + 1;
    }

    return cache[n];
}
