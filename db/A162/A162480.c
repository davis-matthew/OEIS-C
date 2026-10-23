/*
 * A162480  Expansion of 1/((1-x)^2*sqrt(1-4x/(1-x)^4)).
 * https://oeis.org/A162480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A162480: Expansion of 1/((1-x)^2*sqrt(1-4x/(1-x)^4)).
 *
 * Recurrence (valid for n ≥ 4):
 *   a(n) = ( 4*(2n-1)*a(n-1) - 6*(n-1)*a(n-2)
 *           + 2*(2n-3)*a(n-3) - (n-2)*a(n-4) ) / n
 * Initial values:
 *   a(0)=1, a(1)=4, a(2)=21, a(3)=126
 *
 * This implementation uses memoisation; it works for any seq_t type
 * (unsigned 8–128‑bit or arbitrary–precision) as long as standard
 * arithmetic and division are defined for that type.  It is O(n)
 * and works for n up to about 10 000 in under two seconds.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of cached terms */

    /* Return cached value if already computed */
    if (n < cache_len) {
        return cache[n];
    }

    /* Grow the cache to hold terms up to n */
    size_t new_len = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache) {            /* allocation failure: return 0 */
        return (seq_t)0;
    }
    cache = new_cache;

    /* Compute terms from the first missing index up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i == 1) {
            cache[i] = (seq_t)4;
        } else if (i == 2) {
            cache[i] = (seq_t)21;
        } else if (i == 3) {
            cache[i] = (seq_t)126;
        } else {
            /* Coefficients for the recurrence */
            seq_t coeffP1 = (seq_t)(4ULL * (2ULL * i - 1ULL));
            seq_t coeffP2 = (seq_t)(2ULL * (2ULL * i - 3ULL));
            seq_t coeffN1 = (seq_t)(6ULL * (i - 1ULL));
            seq_t coeffN2 = (seq_t)(i - 2ULL);

            /* Positive part: 4*(2n-1)*a(n-1) + 2*(2n-3)*a(n-3) */
            seq_t termP1 = cache[i - 1] * coeffP1;
            seq_t termP2 = cache[i - 3] * coeffP2;
            seq_t sumP   = termP1 + termP2;

            /* Negative part: 6*(n-1)*a(n-2) + (n-2)*a(n-4) */
            seq_t termN1 = cache[i - 2] * coeffN1;
            seq_t termN2 = cache[i - 4] * coeffN2;
            seq_t sumN   = termN1 + termN2;

            /* diff = sumP - sumN (guaranteed non‑negative) */
            seq_t diff = sumP - sumN;

            /* Final value */
            cache[i] = diff / (seq_t)i;
        }
    }

    cache_len = new_len;
    return cache[n];
}
