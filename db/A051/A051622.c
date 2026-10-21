/*
 * A051622  a(n) = (4*n+10)(!^4)/10(!^4), related to A000407 ((4*n+2)(!^4)
 *          quartic, or 4-factorials).
 * https://oeis.org/A051622
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(0) = 1; a(n) = a(n-1) * (4*n + 10)  for n ≥ 1
 * This follows from a(n) = ((4*n+10)(!^4)) / (10(!^4)) which simplifies to
 * a(n+1) = a(n) * (4*n + 14).  Reindexing gives the recurrence above.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoised values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;    /* number of cached terms already computed */
    static idx_t cache_cap = 0;    /* capacity of the cache array */

    /* initialise cache on first call */
    if (cache_len == 0) {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;      /* memory failure – value undefined */
        cache[0] = (seq_t)1;       /* a(0) = 1 */
        cache_len = 1;
    }

    /* extend cache while we need higher terms */
    while (cache_len <= n) {
        if (cache_len >= cache_cap) {            /* grow capacity */
            idx_t new_cap = cache_cap << 1;
            if (new_cap < cache_len + 1)
                new_cap = cache_len + 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;                  /* memory failure – value undefined */
            cache = tmp;
            cache_cap = new_cap;
        }
        /* next term: a(i) = a(i-1) * (4*i + 10) where i = cache_len */
        idx_t i = cache_len;                     /* index for the term to compute */
        seq_t multiplier = (seq_t)(4 * i + 10);
        cache[cache_len] = cache[i - 1] * multiplier;
        cache_len++;
    }

    return cache[n];
}
