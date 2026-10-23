/*
 * A169084  Number of reduced words of length n in Coxeter group on 23
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^25 = I.
 * https://oeis.org/A169084
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of reduced words of length n in Coxeter group *
 *            with generators S_i satisfying S_i^2 = (S_i S_j)^25 = I. * *
 *            Sequence satisfies: for n = 0..24 * a(n) = 2 + 21 *
 *            Σ_{k=0}^{n-1} a(k) (with a(0)=1) * a(25) = 21 * Σ_{k=1}^{24}
 *            a(k) - 230 * and for n ≥ 26 * a(n) = 21 * Σ_{k=1}^{24} a(n-k)...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* array of computed terms */
    static size_t cache_size = 0;    /* number of elements allocated */

    /* first allocation: create space for index 0 */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        cache_size = 0;              /* no term yet */
    }

    /* extend cache up to n, if necessary */
    if ((idx_t)cache_size <= n) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        cache = tmp;
        /* compute any new terms */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;                           /* a(0) = 1 */
            } else if (i <= 24) {                           /* 1 ≤ i ≤ 24 */
                seq_t sum = 0;
                for (idx_t k = 0; k < i; ++k)
                    sum += cache[k];
                cache[i] = (seq_t)2 + (seq_t)21 * sum;           /* a(i) = 2 + 21 * sum */
            } else if (i == 25) {                            /* i = 25 */
                seq_t sum = 0;
                for (idx_t k = 1; k <= 24; ++k)
                    sum += cache[k];
                /* a(25) = 21 * sum - 230  (since cache[0] == 1) */
                cache[25] = (seq_t)21 * sum + (seq_t)1 -
                            (seq_t)231 * cache[0];
            } else { /* i ≥ 26 */
                seq_t sum = 0;
                for (idx_t k = 1; k <= 24; ++k)
                    sum += cache[i - k];
                cache[i] = (seq_t)21 * sum -
                           (seq_t)231 * cache[i - 25];
            }
        }
        cache_size = new_size;
    }

    return cache[n];
}
