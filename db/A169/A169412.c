/*
 * A169412  Number of reduced words of length n in Coxeter group on 15
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^32 = I.
 * https://oeis.org/A169412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A169412: Number of reduced words of length n in Coxeter group
 * with 15 generators S_i (S_i)^2 = (S_i S_j)^32 = I.
 *
 * Recurrence (derived from the rational G.F.):
 *   a(0) = 1
 *   For n ≥ 1:
 *     let b(n) = { 1, if n==0 or n==32
 *                  2, if 1 ≤ n ≤ 31
 *                  0, otherwise }
 *     a(n) = 13 * Σ_{i=1}^{min(31,n)} a(n-i) - 91 * a(n-32) + b(n)
 *
 * The first 17 terms (n ≥ 0) are
 * 1, 15, 210, 2940, 41160, 576240, 8067360, 112943040,
 * 1581202560, 22136835840, 309915701760, 4338819824640,
 * 60743477544960, 850408685629440, 11905721598812160,
 * 166680102383370240, 2333521433367183360.
 *
 * This implementation computes a(n) lazily and memoizes all
 * previously computed terms in a static array.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms currently stored (max index + 1) */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* ensure we have space for indices up to n */
    size_t need_len = (size_t)n + 1;
    if (need_len > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 64;
        while (new_cap < need_len) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* If n already computed, return it */
    if ((size_t)n < cache_len) return cache[(size_t)n];

    /* Compute window sum of last up to 31 terms before starting fresh */
    seq_t window_sum = 0;
    if (cache_len > 0) {
        size_t start = cache_len < 31 ? 0 : cache_len - 31;
        for (size_t i = start; i < cache_len; ++i)
            window_sum += cache[i];
    }

    /* Compute terms from cache_len up to n */
    for (size_t k = cache_len; k <= (size_t)n; ++k) {
        /* b(k) */
        seq_t b = 0;
        if (k == 0 || k == 32) b = 1;
        else if (k <= 31) b = 2;

        /* recurrence */
        seq_t val = 13 * window_sum;
        if (k >= 32)
            val -= 91 * cache[k - 32];
        val += b;

        cache[k] = val;

        /* Update window sum for next iteration */
        window_sum += val;
        if (k >= 31)
            window_sum -= cache[k - 31];
    }

    cache_len = need_len;
    return cache[(size_t)n];
}
