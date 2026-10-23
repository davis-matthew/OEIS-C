/*
 * A021794  Expansion of 1/((1-x)(1-4x)(1-5x)(1-12x)).
 * https://oeis.org/A021794
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n)=22*a(n-1)-149*a(n-2)+368*a(n-3)-240*a(n-4);
 *            a(0)=1,a(1)=22,a(2)=335,a(3)=4460
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation.  cache[i] holds a(i) for 0<=i<=computed */
    static seq_t *cache = NULL;
    static idx_t  cache_size = 0;
    static idx_t  computed   = 0;

    /* Initialise cache on first use with the four base values. */
    if (cache_size == 0)
    {
        cache_size = 4;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)22;
        cache[2] = (seq_t)335;
        cache[3] = (seq_t)4460;
        computed = 3;
    }

    /* If we already know the requested term, return it. */
    if (n <= computed)
        return cache[n];

    /* Grow the allocation if necessary.  Use a doubling strategy. */
    if (n + 1 > cache_size)
    {
        idx_t newsize = cache_size;
        if (newsize == 0) newsize = 1;
        while (newsize <= n) newsize <<= 1;   /* double until large enough */

        seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_size = newsize;
    }

    /* Compute the missing terms up to n using the recurrence. */
    for (idx_t i = computed + 1; i <= n; ++i)
    {
        seq_t v = 22 * cache[i - 1];
        v -= 149 * cache[i - 2];
        v += 368 * cache[i - 3];
        v -= 240 * cache[i - 4];
        cache[i] = v;
    }

    computed = n;
    return cache[n];
}
