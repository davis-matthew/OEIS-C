/*
 * A133649  A007318^(-1) * A133648.
 * https://oeis.org/A133649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-2) for n >= 4, with a(0)=1, a(1)=4,
 *            a(2)=9, a(3)=19. The function memoizes values so it works in
 *            O(n) time per call and can handle any order of indices
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* memoization storage */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;        /* number of cached entries (0..cache_len-1) */
    static idx_t cache_size = 0;       /* allocated size */

    /* return the few known initial values directly */
    if (n < 4) {
        switch (n) {
            case 0: return (seq_t)1;
            case 1: return (seq_t)4;
            case 2: return (seq_t)9;
            case 3: return (seq_t)19;
            default: break;
        }
    }

    /* if the requested index is already cached, return it */
    if (n < cache_len)
        return cache[n];

    /* ensure we have a cache big enough for indices up to n */
    if (cache_len <= n) {
        idx_t needed = n + 1;
        if (cache_size < needed) {
            seq_t *tmp = (seq_t*)realloc(cache, needed * sizeof(seq_t));
            if (!tmp)
                return (seq_t)0;          /* allocation failed */
            cache = tmp;
            cache_size = needed;
        }

        /* initialise the first four values if the cache is empty */
        if (cache_len == 0) {
            cache[0] = (seq_t)1;
            cache[1] = (seq_t)4;
            cache[2] = (seq_t)9;
            cache[3] = (seq_t)19;
            cache_len = 4;
        }

        /* compute the remaining values up to n */
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = cache[i-1] + (cache[i-2] << 1); /* a(i) = a(i-1) + 2*a(i-2) */
        }
        cache_len = n + 1;
    }

    return cache[n];
}
