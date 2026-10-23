/*
 * A168811  Number of reduced words of length n in Coxeter group on 38
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^19 = I.
 * https://oeis.org/A168811
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; * for n≥0: * a(n)=36*∑_{k=1}^{18} a(n-k) – 666*a(n-19) +
 *            r_n * r_n = 1 if n==0 or n==19, * 2 if 1≤n≤18, * 0 otherwise. *
 *            This implements the recurrence obtained from the generating
 *            function * of sequence A168811. The values are stored in a
 *            static cache so that * repeated calls work in lin...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of already computed values */
    static seq_t *cache      = NULL;
    static idx_t cache_size  = 0;

    if (n >= cache_size) {
        /* Allocate space for indices [0 .. n] */
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failure – terminate */
            exit(EXIT_FAILURE);
        }
        cache = tmp;

        /* Compute values from cache_size up to n */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t sum = 0;
            /* Sum the last 18 terms (with coefficient 36) */
            for (idx_t k = 1; k <= 18; ++k) {
                idx_t j = i - k;
                if (j < i) {
                    sum += cache[j] * (seq_t)36ULL;
                }
            }
            /* Subtract 666 times the term 19 positions back */
            idx_t j19 = i - 19;
            if (j19 < i) {
                sum -= cache[j19] * (seq_t)666ULL;
            }

            /* Add the r_n term */
            seq_t r = 0;
            if (i == 0 || i == 19) {
                r = (seq_t)1;
            } else if (i <= 18) {
                r = (seq_t)2;
            }
            cache[i] = sum + r;
        }
        cache_size = n + 1;
    }

    return cache[n];
}
