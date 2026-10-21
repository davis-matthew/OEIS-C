/*
 * A053333  a(n) contains n digits (either '4' or '9') and is divisible by
 *          2^n.
 * https://oeis.org/A053333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = a(n-1) + 10^(n-1) * (4 + 5 * ((a(n-1) / 2^(n-1)) % 2)), *
 *            a(1) = 4. * The function memoises past results and works for any
 *            n >= 1, with any * integer type provided for seq_t and idx_t
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;      /* cache[0] unused, cache[1]==4 */
    static idx_t cache_len = 0;      /* number of cached entries (index 0..cache_len-1) */

    /* if n is already cached, return it immediately */
    if (cache_len > n)
        return cache[n];

    /* initialise for the first time */
    if (cache_len == 0) {
        cache_len = 2;                      /* indices 0 and 1 ready */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = 0;                       /* unused, keep it zero */
        cache[1] = 4;                       /* a(1) = 4 */
    }
    /* extend the cache up to index n */
    if (cache_len <= n) {
        /* allocate larger array */
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache) return 0;           /* allocation failure: return 0 */
        cache = new_cache;

        /* variables holding 10^(i-1) and 2^(i-1) for the last computed i */
        static seq_t pow10 = 1;             /* 10^(current_index-1) */
        static seq_t pow2  = 1;             /* 2^(current_index-1) */

        /* update pow10 and pow2 to correspond to the next index */
        for (idx_t i = cache_len; i <= n; ++i) {
            /* for i > 1 we need 10^(i-1) and 2^(i-1) */
            if (i > 1) {
                pow10 *= 10;                /* 10^(i-1) from 10^(i-2) */
                pow2  <<= 1;                /* 2^(i-1) from 2^(i-2) */
            }

            /* compute a(i) based on a(i-1) */
            seq_t prev = cache[i - 1];

            /* parity of (prev / pow2) */
            seq_t q      = prev / pow2;      /* quotient a(i-1)/2^(i-1) */
            seq_t parity = q & 1;            /* 0 or 1 */
            seq_t digit  = 4 + 5 * parity;   /* 4 or 9 */

            seq_t add = pow10 * digit;       /* 10^(i-1) * digit */
            cache[i] = prev + add;           /* store the new term */
        }
        cache_len = n + 1;
    }
    return cache[n];
}
