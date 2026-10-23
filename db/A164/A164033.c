/*
 * A164033  a(n) = ((4+3*sqrt(2))*(3+sqrt(2))^n +
 *          (4-3*sqrt(2))*(3-sqrt(2))^n)/4.
 * https://oeis.org/A164033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 7*a(n-2) with a(0)=2, a(1)=9
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to allow arbitrary order of calls */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of elements currently cached */

    /* initialise cache with the first two terms */
    if (cache_len == 0)
    {
        cache_len = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (!cache) return 0;          /* out‑of‑memory: return 0 as fallback */
        cache[0] = 2;                  /* a(0) */
        cache[1] = 9;                  /* a(1) */
    }

    /* if requested index already cached, return it */
    if (n < cache_len)
        return cache[n];

    /* extend cache to contain up to index n */
    idx_t new_cap = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return 0;              /* out‑of‑memory: return 0 as fallback */
    cache = tmp;

    /* compute missing terms using recurrence */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        /* recurrence: a(i) = 6*a(i-1) - 7*a(i-2) */
        cache[i] = 6 * cache[i-1] - 7 * cache[i-2];
    }

    cache_len = new_cap;
    return cache[n];
}
