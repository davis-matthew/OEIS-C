/*
 * A045379  Expansion of e.g.f.: exp(4*z + exp(z) - 1).
 * https://oeis.org/A045379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + Sum_{k=0}^{n-1} binomial(n-1,k)*a(k) with a(0)
 *            = 1 (this is the recurrence listed in the OEIS entry)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 21, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialise cache for requested n */
    if (n >= cache_size) {
        /* grow cache to cover n (double strategy) */
        idx_t new_cap = cache_size ? cache_size : 1;
        while (new_cap <= n) new_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;

        /* initialise new slots to zero */
        for (idx_t i = cache_size; i < new_cap; ++i) cache[i] = (seq_t)0;
        cache_size = new_cap;
    }

    /* base case */
    if (n == 0) {
        cache[0] = (seq_t)1;
        return cache[0];
    }

    /* compute up to n */
    for (idx_t i = 1; i <= n; ++i) {
        if (cache[i] != (seq_t)0) continue;   /* already computed */

        /* a(i) = 4*a(i-1) + Sum_{k=0}^{i-1} binomial(i-1,k)*a(k) */
        seq_t sum = 0;
        seq_t binom = 1;               /* binomial(i-1,0) */
        idx_t m = i - 1;

        for (idx_t k = 0; k <= m; ++k) {
            sum += binom * cache[k];
            if (k < m) {
                /* binom = binom * (m - k) / (k + 1) */
                binom = binom * (seq_t)(m - k);
                binom = binom / (seq_t)(k + 1);
            }
        }
        cache[i] = (seq_t)4 * cache[i - 1] + sum;
    }

    return cache[n];
}
