/*
 * A181161  Numerator in abs(binomial(-1/8,n)).
 * https://oeis.org/A181161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(e_2((2*n)!)-n)/n! * Product_{k=0}^{n-1}(8k+1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implemented via the recurrence */
#include <stdlib.h>
#include <stdint.h>

static inline idx_t trailing_zeros(idx_t x) {
    idx_t cnt = 0;
    while ((x & 1) == 0) {
        x >>= 1;
        ++cnt;
    }
    return cnt;
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initialise cache with a(0)=1 */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    if (n < cache_len) {
        return cache[n];
    }

    /* extend cache to the required size */
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) return (seq_t)0;
    cache = tmp;

    for (idx_t i = cache_len; i < new_len; ++i) {
        seq_t prev = cache[i - 1];
        idx_t k = i;              /* compute a(k) from a(k-1) */
        idx_t e = trailing_zeros(k);
        if (sizeof(seq_t) <= 8) {
            /* use 128‑bit intermediate when seq_t is 64‑bit or smaller */
            __int128 t = (__int128)prev * (8 * (k - 1) + 1);
            t <<= e;                 /* multiply by 2^e */
            t /= k;                  /* divide by (k) */
            cache[i] = (seq_t)t;
        } else {
            /* seq_t has enough width (e.g. __int128 or big‑int) */
            seq_t t = prev;
            t *= (seq_t)(8 * (k - 1) + 1);
            t <<= e;
            t /= (seq_t)k;
            cache[i] = t;
        }
    }

    cache_len = new_len;
    return cache[n];
}
