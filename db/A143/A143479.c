/*
 * A143479  Expansion of 1/q(x) where q(x) = x^11*p(x+1/x) and p(x)= -2 -167*x
 *          +1694*x^2 -6069*x^3 +11210*x^4 -12297*x^5 +8554*x^6 -3875*x^7
 *          +1140*x^8 -210*x^9 +22*x^10 - x^11.
 * https://oeis.org/A143479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(0) = -1, and for n ≥ 1 a(n) = ∑_{k=1}^{min(n,22)} q_k ·
 *            a(n‑k), where q_k are the coefficients of q(x) = x¹¹ · p(x +
 *            1/x) and p(t) = -2 -167 t +1694 t² -6069 t³ +11210 t⁴ -12297 t⁵
 *            +8554 t⁶ -3875 t⁷ +1140 t⁸ -210 t⁹ +22 t¹⁰ - t¹¹
 * Range:     i8 n <= 1, i16 n <= 4, i32 n <= 11, i64 n <= 27, i128 n <= 60,
 *            big any n
 * Helpers:   init_q
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/init_q.h"

seq_t a(idx_t n)
{
    /* static state ----------------------------------------------------- */
    static int initialized = 0;
    static seq_t q[23];
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity in seq_t units */
    static size_t cache_len = 0;   /* number of terms computed (>=1) */

    if (!initialized) {
        init_q(q);
        initialized = 1;
    }

    /* ensure the cache is large enough */
    size_t need = (size_t)n + 1;
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        if (new_cap < need)
            new_cap = need;
        seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (cache && cache_len > 0)
            memcpy(new_cache, cache, cache_len * sizeof(seq_t));
        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute any missing terms up to n */
    if (cache_len <= n) {
        /* a(0) is 1/q_0; with q_0 == -1, this is -1 */
        if (cache_len == 0) {
            cache[0] = (seq_t)1 / q[0];
            cache_len = 1;
        }
        /* compute from current length to n */
        for (size_t m = cache_len; m <= (size_t)n; ++m) {
            seq_t sum = 0;
            size_t maxk = (m < 22) ? m : 22;
            for (size_t k = 1; k <= maxk; ++k)
                sum += q[(int)k] * cache[m - k];
            cache[m] = sum;
        }
        cache_len = need;
    }

    return cache[n];
}
