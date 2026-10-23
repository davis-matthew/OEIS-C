/*
 * A170524  Number of reduced words of length n in Coxeter group on 35
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^46 = I.
 * https://oeis.org/A170524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 33*sum_{i=1}^{min(45,n)} a(n-i) - 561*a(n-46) + p(n)
 *            Where p(n) = 2 for 1 ≤ n ≤ 45, p(46) = 1, and p(n) = 0
 *            otherwise. This recurrence follows from the rational generating
 *            function of the Coxeter group growth series
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_small.h"   /* provides seq_t mul_small(seq_t, unsigned int) */

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initialise cache with a(0) = 1 if needed */
    if (cache_len == 0) {
        cache_len = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        cache[0] = (seq_t)1;
    }

    /* extend cache up to n */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) {
            /* allocation failed; abort */
            return (seq_t)0;
        }
        cache = new_cache;
        /* compute new terms */
        for (idx_t i = cache_len; i <= n; ++i) {
            /* compute S = sum_{k=max(0,i-45)}^{i-1} cache[k] */
            seq_t S = 0;
            idx_t start = (i > 45) ? (i - 45) : 0;
            for (idx_t k = start; k < i; ++k) {
                S += cache[k];
            }
            /* a_back = cache[i-46] if i>=46, else 0 */
            seq_t a_back = 0;
            if (i >= 46) {
                a_back = cache[i - 46];
            }
            /* T = S - 17*a_back */
            seq_t T = S - mul_small(a_back, 17);
            /* value = 33*T + p(i) */
            seq_t value = mul_small(T, 33);
            seq_t p = 0;
            if (i <= 45) {
                p = (seq_t)2;
            } else if (i == 46) {
                p = (seq_t)1;
            }
            value += p;
            cache[i] = value;
        }
        cache_len = new_len;
    }

    return cache[n];
}
