/*
 * A042299  Denominators of continued fraction convergents to sqrt(675).
 * https://oeis.org/A042299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 52*a(n-2) - a(n-4) for n ≥ 4; A(0) = 1, A(1) = 1, A(2) =
 *            51, A(3) = 52
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cached values of a(0)..a(cache_len-1) */
    static idx_t cache_len = 0;      /* number of elements stored (max index + 1) */

    /* Small indices are returned directly */
    if (n < 4) {
        switch (n) {
        case 0: return (seq_t)1;
        case 1: return (seq_t)1;
        case 2: return (seq_t)51;
        case 3: return (seq_t)52;
        default: /* unreachable */ return (seq_t)0;
        }
    }

    /* If the cache does not contain the requested index, grow and fill it */
    if (cache_len <= n) {
        if (cache_len == 0) {
            /* First allocation: create space up to n and compute all values */
            idx_t new_len = n + 1;
            cache = (seq_t *)malloc(new_len * sizeof(seq_t));
            if (!cache) abort();               /* on allocation failure */
            cache[0] = (seq_t)1;
            cache[1] = (seq_t)1;
            cache[2] = (seq_t)51;
            cache[3] = (seq_t)52;
            for (idx_t i = 4; i <= n; ++i) {
                cache[i] = (seq_t)52 * cache[i-2] - cache[i-4];
            }
            cache_len = new_len;
        } else {
            /* Grow the existing cache to accommodate index n */
            idx_t old_len = cache_len;
            idx_t new_len = n + 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            for (idx_t i = old_len; i <= n; ++i) {
                cache[i] = (seq_t)52 * cache[i-2] - cache[i-4];
            }
            cache_len = new_len;
        }
    }

    /* Return the cached value */
    return cache[n];
}
