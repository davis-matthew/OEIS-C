/*
 * A026658  Sum{T(n-k,k)}, 0<=k<=[ n/2 ], T given by A026648.
 * https://oeis.org/A026658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A026658
 *  Formula:  a(0)=1, a(1)=1, a(2)=2, a(3)=4, a(4)=6, a(5)=11,
 *            a(6)=17, a(7)=30, and for n≥8
 *            a(n) = 2*a(n-2) + a(n-4) + 4*a(n-6) – a(n-8)
 *  (Recurrence derived from the generating function
 *   (−x⁶+2x⁵+x⁴+2x³+x+1)/(x⁸−4x⁶−x⁴−2x²+1).)
 *  This implementation memoises the sequence in a static array.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: indices 0 … cache_len-1 are stored */
    static seq_t *cache    = NULL;
    static size_t cache_len = 0;   /* actual number of valid terms */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* base values for n = 0 … 7 */
    static const seq_t init[8] = { 1, 1, 2, 4, 6, 11, 17, 30 };

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 8;
        cache_len = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0; /* allocation failure */
        for (size_t i = 0; i < 8; ++i) cache[i] = init[i];
    }

    /* if requested index is already cached, return it */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* grow cache to at least n+1 elements */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        /* double capacity until sufficient */
        while (cache_cap < needed) cache_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (tmp == NULL) return 0; /* allocation failure */
        cache = tmp;
    }

    /* compute new terms using the recurrence */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        /* Since i ≥ 8, all needed indices are valid */
        seq_t term = 2 * cache[i - 2]          /* 2*a(i-2) */
                   + cache[i - 4]              /* + a(i-4) */
                   + 4 * cache[i - 6]          /* + 4*a(i-6) */
                   - cache[i - 8];             /* - a(i-8) */
        cache[i] = term;
    }

    cache_len = needed;
    return cache[(size_t)n];
}
