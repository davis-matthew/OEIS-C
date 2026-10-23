/*
 * A033747  Expansion of Product_{d | 35} theta_3(q^d).
 * https://oeis.org/A033747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of q^n in the product ∏_{d|35} θ₃(q^d)
 * Range:     u8 n <= 335, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This uses the definition θ₃(q) = 1 + 2∑_{m≥1} q^{m²} */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of values stored  */

    /* If we have not yet computed up to n, rebuild the cache */
    if (n + 1 > cache_len)
    {
        idx_t new_len = n + 1;
        idx_t i, r, j, k;
        /* arrays for convolution */
        seq_t *cur  = (seq_t *)malloc(new_len * sizeof(seq_t));
        seq_t *next = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!cur || !next) { /* handle allocation failure */
            return (seq_t)0;
        }

        /* initialise current product to 1 at exponent 0 */
        for (i = 0; i < new_len; ++i)
            cur[i] = (seq_t)0;
        cur[0] = (seq_t)1;

        /* divisors of 35 */
        const idx_t divisors[] = {1,5,7,35};
        const int nd = sizeof(divisors)/sizeof(divisors[0]);

        for (int d_idx = 0; d_idx < nd; ++d_idx)
        {
            idx_t d = divisors[d_idx];
            /* zero next array */
            for (i = 0; i < new_len; ++i)
                next[i] = (seq_t)0;

            /* convolve cur with the series for θ₃(q^d) */
            for (r = 0; ; ++r)
            {
                k = d * r * r;
                if (k >= new_len)
                    break;
                seq_t coeff = (r == 0) ? (seq_t)1 : (seq_t)2;
                idx_t max_j = new_len - 1 - k;
                for (j = 0; j <= max_j; ++j)
                {
                    if (cur[j] != (seq_t)0)
                        next[j + k] += cur[j] * coeff;
                }
            }
            /* swap cur and next for next iteration */
            seq_t *tmp = cur;
            cur = next;
            next = tmp;
        }

        /* copy result into cache */
        if (cache)
            free(cache);
        cache = cur;
        cache_len = new_len;
        free(next);   /* next is just the temporary array we swapped with */

    }

    return cache[n];
}
