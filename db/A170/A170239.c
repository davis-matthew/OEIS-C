/*
 * A170239  Number of reduced words of length n in Coxeter group on 38
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^40 = I.
 * https://oeis.org/A170239
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) derives from the rational generating function G(t) = (1 +
 *            2∑t^k(1≤k≤39)+t^40) / (1 – 36∑t^k(1≤k≤39)+666 t^40). Thus for
 *            all n ≥ 0 a(n) = 36·∑_{k=1}^{39} a(n-k) – 666·a(n-40) + c_n,
 *            where c_0 = 1, c_1 … c_39 = 2, c_40 = 1 and c_n = 0 for n > 40
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>   /* for malloc, realloc, abort */

seq_t a(idx_t n)
{
    /* cache holds all previously computed terms a[0..cache_len-1] */
    static seq_t *cache      = NULL;
    static idx_t  cache_len  = 0;   /* number of elements currently stored */

    /* grow the cache if necessary and compute new terms */
    if (n + 1 > cache_len) {
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – terminate */
            abort();
        }
        cache = tmp;

        /* compute terms from old_len up to n */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t coeff;
            if (i == 0 || i == 40) {
                coeff = (seq_t)1;
            } else if (i >= 1 && i <= 39) {
                coeff = (seq_t)2;
            } else {
                coeff = (seq_t)0;
            }

            /* sum of the 39 preceding terms (or fewer if i < 39) */
            seq_t sum = 0;
            idx_t maxk = (i < 39 ? i : 39);
            for (idx_t k = 1; k <= maxk; ++k) {
                sum += cache[i - k];
            }

            /* compute a(i) using the recurrence */
            seq_t val = coeff + (seq_t)36 * sum;
            if (i >= 40) {
                val -= (seq_t)666 * cache[i - 40];
            }

            cache[i] = val;
        }
        cache_len = n + 1;
    }
    return cache[n];
}
