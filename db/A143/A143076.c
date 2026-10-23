/*
 * A143076  Sequence from expansion of Cartan E_11 12 state root sum zero
 *          characteristic polynomial: p(x)=1/(-1 + 274 x^2 - 3480 x^3 + 21205
 *          x^4 - 76696 x^5 + 175891x^6 - 259324 x^7 + 240551 x^8 - 131824 x^9
 *          + 37101 x^10 - 3676 x^11 - 44 x^12).
 * https://oeis.org/A143076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  uned, sign
 * Formula:   a(n) = Sum_{k=2..12} c_k * a(n-k) with initial values a(1) = -1,
 *            a(2) = 0. The coefficients c_k come from the polynomial -1 + 274
 *            x^2 - 3480 x^3 + 21205 x^4 - 76696 x^5 + 175891 x^6 - 259324 x^7
 *            + 240551 x^8 - 131824 x^9 + 37101 x^10 - 3676 x^11 - 44 x^12
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 8, i64 n <= 15, i128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;        /* undefined; return 0 */

    /* Coefficients c_k for k = 0..12 (c_0 and c_1 unused). */
    static const seq_t c[13] = {
        0, 0,                                        /* c_0, c_1 */
        274, -3480, 21205, -76696, 175891, -259324,   /* c_2..c_7 */
        240551, -131824, 37101, -3676, -44            /* c_8..c_12 */
    };

    /* Static cache for previously computed terms. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of terms stored */
    static idx_t cache_cap = 0;   /* allocated capacity */

    /* Initialize cache on first call. */
    if (cache_len == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)-1;      /* a(1) */
        cache[1] = (seq_t)0;       /* a(2) */
        cache_len = 2;
    }

    /* Index in cache corresponding to a(n). */
    idx_t idx = n - 1;

    /* Extend cache if necessary. */
    if (idx >= cache_len) {
        /* Grow capacity. */
        if (idx + 1 > cache_cap) {
            while (cache_cap <= idx + 1)
                cache_cap <<= 1;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        /* Compute terms from cache_len up to idx. */
        for (idx_t i = cache_len; i <= idx; ++i) {
            seq_t sum = (seq_t)0;
            for (int k = 2; k <= 12; ++k) {
                if (i >= (idx_t)k)
                    sum += c[k] * cache[i - k];
            }
            cache[i] = sum;
        }
        cache_len = idx + 1;
    }

    return cache[idx];
}
