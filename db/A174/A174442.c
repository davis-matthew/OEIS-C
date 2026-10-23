/*
 * A174442  a(n) = a(n-1)+a(n-2) if n is odd, otherwise a(n) = a(n-1)*a(n-2)
 *          with a(1)=3 and a(2)=5.
 * https://oeis.org/A174442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 3, a(2) = 5, a(n) = a(n-1)+a(n-2) if n is odd, a(n) =
 *            a(n-1)*a(n-2) if n is even
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 9, u64 n <= 11, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle indices < 1 gracefully */
    if (n == 0)
        return (seq_t)0;

    /* Static cache to support out‑of‑order calls */
    static seq_t *cache = NULL;
    static size_t cache_sz = 0;   /* number of computed elements */

    /* Ensure that cache has base values a(1)=3, a(2)=5 */
    if (cache_sz < 2) {
        seq_t *tmp = (seq_t *)realloc(cache, 2 * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;      /* allocation failed */
        cache = tmp;
        cache[0] = (seq_t)3;     /* a(1) */
        cache[1] = (seq_t)5;     /* a(2) */
        cache_sz = 2;
    }

    /* If requested term already computed, return it */
    if ((size_t)n <= cache_sz)
        return cache[(size_t)n - 1];

    /* Grow cache to hold terms up to n */
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
    if (!tmp)
        return (seq_t)0;          /* allocation failed */
    cache = tmp;

    /* Compute missing terms iteratively */
    for (idx_t i = (idx_t)cache_sz + 1; i <= n; ++i) {
        size_t idx   = (size_t)i - 1;      /* 0‑based index in cache */
        size_t prev1 = idx - 1;            /* i-1 term */
        size_t prev2 = idx - 2;            /* i-2 term */
        if ((i & 1) == 1)                  /* odd i: addition */
            cache[idx] = cache[prev1] + cache[prev2];
        else                               /* even i: multiplication */
            cache[idx] = cache[prev1] * cache[prev2];
    }

    cache_sz = (size_t)n;
    return cache[(size_t)n - 1];
}
