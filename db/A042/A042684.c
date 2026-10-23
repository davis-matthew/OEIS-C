/*
 * A042684  Numerators of continued fraction convergents to sqrt(872).
 * https://oeis.org/A042684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 252006*a(n-10) - a(n-20) (n >= 20) The first 20 terms are
 *            the numerators of the convergents to sqrt(872): a(0) = 29; a(1)
 *            = 30; a(2) = 59; a(3) = 443; a(4) = 502; a(5) = 6969; a(6) =
 *            7471; a(7) = 59266; a(8) = 66737; a(9) = 126003; a(10) =
 *            7374911; a(11) = 7500914; a(12) = 14875825; a(13)...
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 16, u64 n <= 34, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static cache for computed terms, supports arbitrary call order. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;    /* number of elements stored */
    static size_t capacity = 0;     /* allocated capacity */

    /* If the cache is not initialized yet, compute the first 20 terms. */
    if (cache == NULL) {
        /* Allocate at least enough space for 20 terms, or for the requested
           term if it is larger. */
        capacity = (n >= 20) ? (size_t)n + 1 : 20;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failure: return 0 */

        /* Compute the partial quotients of the continued fraction of sqrt(872). */
        const uint64_t D = 872ULL;
        const uint64_t a0 = 29ULL;   /* floor(sqrt(872)) */
        uint64_t af[20];
        uint64_t m = 0, d = 1;
        af[0] = a0;
        for (size_t i = 1; i < 20; ++i) {
            m = d * af[i - 1] - m;
            d = (D - m * m) / d;
            af[i] = (a0 + m) / d;
        }

        /* Compute the first 20 convergent numerators using the standard
           numerator recurrence: p[-2] = 0, p[-1] = 1. */
        seq_t p_prev2 = 0;           /* p[-2] */
        seq_t p_prev1 = 1;           /* p[-1] */
        for (size_t i = 0; i < 20; ++i) {
            seq_t p = (seq_t)af[i] * p_prev1 + p_prev2;
            cache[i] = p;
            p_prev2 = p_prev1;
            p_prev1 = p;
        }
        cache_len = 20;
    }

    /* Ensure the cache has space for the desired index. */
    if ((idx_t)n >= capacity) {
        size_t new_cap = capacity ? capacity : 256;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;      /* allocation failure */
        cache = tmp;
        capacity = new_cap;
    }

    /* Compute any missing terms up to n (in ascending order). */
    for (idx_t m = cache_len; m <= (idx_t)n; ++m) {
        seq_t val;
        if (m < 20) {
            /* This case should not happen because the first 20 terms are
               already filled, but we guard just in case. */
            val = cache[m];
        } else {
            val = ((seq_t)252006) * cache[(size_t)(m - 10)] - cache[(size_t)(m - 20)];
        }
        cache[(size_t)m] = val;
    }
    cache_len = (size_t)(n + 1);

    return cache[(size_t)n];
}
