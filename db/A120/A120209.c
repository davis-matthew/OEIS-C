/*
 * A120209  a(1)=8; a(n)=floor((79+sum(a(1) to a(n-1)))/9).
 * https://oeis.org/A120209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=8; a(n)=floor((79+sum_{i=1}^{n-1} a(i))/9)
 * Range:     u8 n <= 15, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
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
    static idx_t cache_size = 0;   /* number of terms stored */
    static seq_t cache_sum = 0;    /* sum of cached terms */

    if (n == 0)
        return 0; /* undefined for n=0, but harmless */

    /* grow cache if needed */
    if (cache_size < n)
    {
        size_t new_cap = (cache_size > 0 ? cache_size * 2 : 64);
        if (new_cap < (size_t)n)
            new_cap = (size_t)n;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        /* cache_size remains unchanged until we fill new entries */
    }

    /* compute missing terms */
    for (idx_t i = cache_size + 1; i <= n; ++i)
    {
        seq_t val = (cache_sum + ((seq_t)79)) / ((seq_t)9);
        cache[i - 1] = val;
        cache_sum += val;
    }
    cache_size = n;   /* cache now contains 1..n */

    return cache[n - 1];
}
