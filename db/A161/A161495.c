/*
 * A161495  Expansion of x*(3*x-1)*(x-3)/(1-15*x+32*x^2-15*x^3+x^4).
 * https://oeis.org/A161495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*a(n-1) - 32*a(n-2) + 15*a(n-3) - a(n-4), with a(1)=3,
 *            a(2)=35, a(3)=432, a(4)=5405
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache[0] = a(1), cache[1] = a(2), … */
    static idx_t   cache_len = 0; /* number of values stored in cache */

    if (n == 0)                /* offset is 1 – requests for n==0 never
                                 occur in the tests, but we return 0. */
        return (seq_t)0;

    /* initialise the first four terms once */
    if (cache_len == 0) {
        cache_len = 4;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache[0] = (seq_t)3;         /* a(1) */
        cache[1] = (seq_t)35;        /* a(2) */
        cache[2] = (seq_t)432;       /* a(3) */
        cache[3] = (seq_t)5405;      /* a(4) */
    }

    /* if we already have the answer, just return it */
    if (n <= cache_len)
        return cache[n - 1];

    /* grow the cache to size n */
    seq_t *new_cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);
    cache = new_cache;

    /* compute missing terms using the recurrence */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* a(i) = 15*a(i-1) - 32*a(i-2) + 15*a(i-3) - a(i-4) */
        seq_t term1 = (seq_t)15 * cache[i - 2]; /* a(i-1) */
        seq_t term2 = (seq_t)32 * cache[i - 3]; /* a(i-2) */
        seq_t term3 = (seq_t)15 * cache[i - 4]; /* a(i-3) */
        seq_t term4 = cache[i - 5];            /* a(i-4) */
        cache[i - 1] = term1 - term2 + term3 - term4;
    }

    cache_len = n;
    return cache[n - 1];
}
