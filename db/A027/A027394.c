/*
 * A027394  Number of labeled servers of dimension 7.
 * https://oeis.org/A027394
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of labeled servers of dimension 7 Equivalent
 *            recurrence: a(0) = 1 a(n) = Σ_{k=1}^{min(n,7)} C(7,k) *
 *            (n-1)!/(n-k)! * a(n-k) where C(7,k) are binomial coefficients.
 *            This follows from the exponential generating function exp(
 *            Σ_{i=1}^{7} ((1+x)^i-1)/i )
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t C7[8] = {0, 7, 21, 35, 35, 21, 7, 1};

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t  cache_cap = 0;
    static idx_t  computed_upto = 0;

    /* ensure capacity */
    if (cache == NULL) {
        cache_cap = n + 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (cache == NULL) return 0;       /* allocation failure – unlikely */
        for (idx_t i = 0; i < cache_cap; ++i) cache[i] = 0;
        cache[0] = (seq_t)1;
        computed_upto = 0;
    } else if (cache_cap <= n) {
        idx_t old_cap = cache_cap;
        cache_cap = n + 1;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
        if (cache == NULL) return 0;       /* allocation failure – unlikely */
        for (idx_t i = old_cap; i < cache_cap; ++i) cache[i] = 0;
    }

    /* compute values up to n */
    if (computed_upto < n) {
        for (idx_t m = computed_upto + 1; m <= n; ++m) {
            idx_t maxK = (m < 7 ? m : 7);
            seq_t sum = 0;
            for (idx_t k = 1; k <= maxK; ++k) {
                seq_t prod = (seq_t)1;
                /* prod = Π_{t=1}^{k-1} (m - t) = (m-1)!/(m-k)! */
                for (idx_t t = 1; t < k; ++t)
                    prod *= (seq_t)(m - t);
                seq_t term = prod * (seq_t)C7[k] * cache[m - k];
                sum += term;
            }
            cache[m] = sum;
        }
        computed_upto = n;
    }

    return cache[n];
}
