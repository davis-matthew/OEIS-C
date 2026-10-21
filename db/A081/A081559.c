/*
 * A081559  Expansion of e.g.f.: exp(cosh(2*x)-1), even powers only.
 * https://oeis.org/A081559
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1; For n ≥ 1: a(n) = Σ_{m=1}^{n} binom(2n‑1, 2m‑1) · 4^m
 *            · a(n‑m) where binom(N, K) = C(N,K). This follows from the
 *            e.g.f. exp(cosh(2x)-1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of values stored (cache_len > 0) */
    static idx_t cache_cap = 0;

    /* initialisation */
    if (cache_len == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;   /* out of memory – return 0 as a safe sentinel */
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* grow cache if needed */
    if (cache_len <= n) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing values up to n */
    for (idx_t cur = cache_len; cur <= n; ++cur) {
        if (cur == 0) {
            cache[0] = (seq_t)1;
            continue;
        }

        seq_t sum = (seq_t)0;
        seq_t C = (seq_t)(2 * cur - 1);       /* binom(2*cur-1, 1) */
        seq_t pow4 = (seq_t)4;                /* 4^m with m=1 */
        seq_t N = (seq_t)(2 * cur - 1);       /* constant for all iterations */

        for (idx_t m = 1; m <= cur; ++m) {
            seq_t temp = pow4 * cache[cur - m];
            temp *= C;
            sum += temp;

            if (m == cur) break;             /* last term, no further update */

            /* update C to binom(N, 2*(m+1)-1) = binom(N, 2m+1) */
            seq_t num1 = (seq_t)(N - (2 * m - 1));       /* N - k   */
            seq_t num2 = (seq_t)(N - (2 * m - 1) - 1);   /* N - k - 1 */
            seq_t den1 = (seq_t)(2 * m);                 /* k + 1   */
            seq_t den2 = (seq_t)(2 * m + 1);             /* k + 2   */
            C = (C * num1 * num2) / (den1 * den2);

            pow4 <<= 2;                                 /* multiply by 4 */
        }

        cache[cur] = sum;
    }

    cache_len = n + 1;
    return cache[n];
}
