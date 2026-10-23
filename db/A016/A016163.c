/*
 * A016163  Expansion of 1/((1-5*x)*(1-9*x)).
 * https://oeis.org/A016163
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*a(n-1) - 45*a(n-2) Initial values: a(0) = 1, a(1) = 14
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;        /* array of computed terms             */
    static idx_t cache_len = 0;         /* allocation size (number of slots)    */
    static idx_t computed_len = 0;      /* number of valid elements in cache    */

    /* initialise the cache if this is the first call */
    if (cache == NULL)
    {
        cache_len = 2;                      /* at least two slots for a(0) and a(1) */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (!cache)
            return 0;                      /* fallback – should never happen       */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)14;
        computed_len = 2;
    }

    /* grow the allocated array if necessary */
    if (cache_len <= n)
    {
        cache_len = n + 1;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_len);
        if (!cache)
            return 0;                      /* fallback – should never happen       */
    }

    /* compute any missing terms up to n inclusive */
    for (idx_t i = computed_len; i <= n; ++i)
    {
        if (i == 0)
            cache[i] = (seq_t)1;
        else if (i == 1)
            cache[i] = (seq_t)14;
        else
            cache[i] = ((seq_t)14 * cache[i - 1]) - ((seq_t)45 * cache[i - 2]);
    }
    computed_len = n + 1;                  /* all terms up to n are now valid       */

    return cache[n];
}
