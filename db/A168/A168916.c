/*
 * A168916  Number of reduced words of length n in Coxeter group on 47
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^21 = I.
 * https://oeis.org/A168916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(n) + 45 * sum_{k=1}^{20} a(n-k) - 1035 * a(n-21), *
 *            where p(0)=1, p(1)…p(20)=2, p(21)=1, and p(n)=0 for n>21. * This
 *            recurrence follows from the generating function * G(t) = (t^21 +
 *            2 t^20 + … + 2 t + 1)/(1 - 45 t - … - 45 t^20 + 1035 t^21). *
 *            The sequence starts with a(0)=1, a(1)=47,
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache of previously computed terms.
       cache_len holds the number of elements allocated (>= computed count). */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of terms stored in cache */
    static idx_t computed = 0;    /* number of terms computed so far (cache[0..computed-1] valid) */

    /* Extend the cache if necessary. */
    if (n >= computed) {
        idx_t old_len = cache_len;
        idx_t new_len = n + 1;
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        }
        if (cache == NULL) {
            /* Allocation failed; in the context of this library this is unlikely. */
            return (seq_t)0;
        }
        cache_len = new_len;

        /* Compute terms from 'computed' up to 'n'. */
        for (idx_t i = computed; i <= n; ++i) {
            seq_t coeff;
            if (i == 0 || i == 21) {
                coeff = 1;
            } else if (i >= 1 && i <= 20) {
                coeff = 2;
            } else {
                coeff = 0;
            }

            /* Sum of the previous 20 terms. */
            seq_t sum = 0;
            for (idx_t k = 1; k <= 20 && i >= k; ++k) {
                sum += cache[i - k];
            }

            /* Previous 21st term (0 if i < 21). */
            seq_t a_21_back = (i >= 21) ? cache[i - 21] : 0;

            /* Compute a[i] using the recurrence. */
            seq_t term = coeff;
            term += (seq_t)45 * sum;
            term -= (seq_t)1035 * a_21_back;

            cache[i] = term;
        }
        computed = n + 1;  /* All terms up to n are now valid. */
    }

    return cache[n];
}
