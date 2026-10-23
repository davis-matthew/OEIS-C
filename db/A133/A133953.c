/*
 * A133953  Expansion of 2*x*(1+x)*(1-2*x)/((1-x)^2*(1-2*x-x^2)).
 * https://oeis.org/A133953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 4*a(n-1) - 4*a(n-2) + a(n-4) (n ≥ 5) Initial values
 *            (offset 1): a(1) = 2, a(2) = 6, a(3) = 12, a(4) = 24
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 51, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)          /* not defined for offset 1, return 0 */
        return (seq_t)0;

    static seq_t *cache      = NULL; /* cache[i-1] = a(i)  for i ≥ 1 */
    static size_t cache_cap  = 0;     /* allocated size of cache */
    static idx_t  cache_last = 0;     /* highest index that has been computed */

    /* Ensure the cache can store terms up to n. */
    if ((size_t)n > cache_cap) {
        size_t new_cap = (size_t)n;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)
            abort();               /* allocation failure */
        cache     = new_cache;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to n on the fly. */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1)
            val = (seq_t)2;
        else if (i == 2)
            val = (seq_t)6;
        else if (i == 3)
            val = (seq_t)12;
        else if (i == 4)
            val = (seq_t)24;
        else { /* recurrence for i ≥ 5 */
            val = (seq_t)4 * cache[i-2]      /* a(i-1) */
                - (seq_t)4 * cache[i-3]      /* a(i-2) */
                + cache[i-5];                /* a(i-4) */
        }
        cache[i-1] = val; /* store a(i) */
    }

    cache_last = n;
    return cache[n-1];
}
