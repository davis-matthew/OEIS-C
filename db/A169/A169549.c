/*
 * A169549  Number of reduced words of length n in Coxeter group on 8
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^35 = I.
 * https://oeis.org/A169549
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A169549: Number of reduced words of length n in the Coxeter group
 *  with 8 generators S_i and relations (S_i)^2 = (S_i S_j)^35 = I.
 *
 *  The ordinary generating function is
 *      G(t) = (t^35 + 2 t^34 + … + 2 t + 1) /
 *             (21 t^35 - 6 t^34 - … - 6 t + 1)
 *
 *  From this we obtain the linear recurrence (valid for n ≥ 35):
 *
 *      a(n) = 6·(a(n‑1)+a(n‑2)+…+a(n‑34)) - 21·a(n‑35)
 *
 *  The first 35 terms are determined uniquely by G(t); they are computed
 *  from the identity   G(t)·D(t) = N(t).  For 0 ≤ n < 35 we have
 *
 *      a(n) = N(n) + 6·(a(0)+a(1)+…+a(n‑1)),
 *      where N(n) = 1 for n = 0, 2 for 1 ≤ n ≤ 34, and N(35) = 1.
 *
 *  The function below memoises the sequence in a static cache, so that
 *  subsequent calls with larger indices can be answered in O(1) after
 *  a single O(n) precomputation.  It works with any implementation of
 *  seq_t/idx_t provided by the build (fixed‑width or arbitrary precision).
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache state */
    static seq_t *cache = NULL;     /* array of a(n) */
    static size_t cache_len = 0;    /* first unused index   */
    static size_t cache_cap = 0;    /* allocated size       */

    /* ensure array is large enough for a(n) */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 64;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* precompute terms up to n if necessary */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if (i < 35) {
            /* a(i) = 2 + 6 * sum_{k=0}^{i-1} a(k)   (i>=1) */
            seq_t sum = 0;
            for (size_t k = 0; k < i; ++k)
                sum += cache[k];
            cache[i] = (seq_t)2 + ((seq_t)6 * sum);
        } else {
            /* recurrence for i >= 35:
             * a(i) = 6 * (a(i-1)+ … + a(i-34)) - 21 * a(i-35) */
            seq_t sum34 = 0;
            for (int k = 1; k <= 34; ++k)
                sum34 += cache[i - k];
            seq_t term1 = (seq_t)6 * sum34;
            seq_t term2 = (seq_t)21 * cache[i - 35];
            cache[i] = term1 - term2;
        }
    }
    cache_len = (size_t)n + 1;
    return cache[n];
}
