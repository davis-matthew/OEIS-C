/*
 * A170616  Number of reduced words of length n in Coxeter group on 31
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^48 = I.
 * https://oeis.org/A170616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 29 * Σ_{i=1}^{47} a(n-i) - 435 * a(n-48) + p_n where p_0
 *            = 1, p_k = 2 for 1 ≤ k ≤ 47, p_48 = 1, and p_k = 0 otherwise.
 *            The recurrence follows from the rational generating function
 *            (t^48 + 2*t^47 + … + 2*t + 1)/(435*t^48 - 29*t^47 - … - 29*t +
 *            1). The values for n < 48 automatically include...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;    /* number of elements stored */

    /* extend cache if needed */
    if (n + 1 > cache_len) {
        /* allocate new space */
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) {
            /* allocation failed; return 0 as sentinel (should not happen) */
            return (seq_t)0;
        }
        cache = new_cache;

        /* compute sum of last up to 47 terms for sliding window */
        seq_t sum47 = 0;
        idx_t start = 0;
        if (cache_len >= 47) {
            start = cache_len - 47;
        }
        for (idx_t j = start; j < cache_len; ++j) {
            sum47 += cache[j];
        }

        /* compute new terms from cache_len up to n */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t base = sum47 * (seq_t)29;          /* 29 * sum of last 47 */

            if (i >= 48) {
                base -= cache[i - 48] * (seq_t)435;  /* - 435 * a(i-48) */
            }

            /* add p_i */
            if (i == 0 || i == 48) {
                base += (seq_t)1;
            } else if (i <= 47 && i >= 1) {
                base += (seq_t)2;
            }

            cache[i] = base;

            /* update sliding window sum */
            sum47 += cache[i];
            if (i >= 47) {
                sum47 -= cache[i - 47];
            }
        }

        cache_len = new_len;
    }

    return cache[n];
}
