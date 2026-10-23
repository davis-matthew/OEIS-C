/*
 * A170502  Number of reduced words of length n in Coxeter group on 13
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^46 = I.
 * https://oeis.org/A170502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = coeff_N(n) + 11 * sum_{k=1}^{45} a(n-k) - 66 * a(n-46)
 * where  coeff_N(0)=1, coeff_N(1..45)=2, coeff_N(46)=1, coeff_N(n)=0 for n>46.
 * Also a(i)=0 for i<0, and a(0)=1.
 *
 * This recurrence is derived from the ordinary generating function
 * G(t) = (t^46 + 2*t^45 + … + 2*t + 1) / (66*t^46 - 11*t^45 - … - 11*t + 1).
 * The coefficient of t^n in G(t) is the desired sequence value a(n).
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    /* static cache of previously computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n >= cache_size) {
        /* grow cache to hold a(0..n) */
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_cache) return 0; /* allocation failure (unlikely in tests) */

        /* copy old data, free old cache */
        if (cache) {
            memcpy(new_cache, cache, cache_size * sizeof(seq_t));
            free(cache);
        }
        cache = new_cache;

        /* initialize newly allocated elements */
        if (cache_size == 0) {
            cache[0] = 1;            /* a(0) = 1   */
            cache_size = 1;
        }

        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t rhs;
            if (i == 0 || i == 46) {
                rhs = 1;
            } else if (i <= 45) {
                rhs = 2;
            } else {
                rhs = 0;
            }

            seq_t sum = 0;
            for (idx_t k = 1; k <= 45 && i >= k; ++k) {
                sum += cache[i - k];
            }

            seq_t term46 = 0;
            if (i >= 46) {
                term46 = cache[i - 46];
            }

            cache[i] = rhs + (seq_t)11 * sum - (seq_t)66 * term46;
        }
        cache_size = new_size;
    }

    return cache[n];
}
