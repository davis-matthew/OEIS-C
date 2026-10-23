/*
 * A170425  Number of reduced words of length n in Coxeter group on 32
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^44 = I.
 * https://oeis.org/A170425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 30*(a(n-1)+a(n-2)+...+a(n-43)) - 465*a(n-44) + N(n) where
 *            N(0)=1, N(1..43)=2, N(44)=1, N(n>44)=0
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: a(0) = 1.  N(n) is the coefficient of the numerator
   in the generating function. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cache = NULL;
    static size_t max_computed = 0; /* largest index already computed */

    /* allocate and initialise cache if first call */
    if (cache == NULL) {
        cache = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        if (cache == NULL) return 0;          /* out of memory – fail quietly */
        cache[0] = (seq_t)1;                 /* a(0) = 1 */
        max_computed = 0;
    }
    /* if needed, grow cache and compute missing terms */
    else if (n > max_computed) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return 0;          /* out of memory – fail quietly */
        cache = tmp;

        for (size_t i = max_computed + 1; i <= (size_t)n; ++i) {
            seq_t sum = (seq_t)0;

            /* sum 30 * a[i - k] for k = 1 .. min(43, i) */
            size_t max_k = (i < 43 ? i : 43);
            for (size_t k = 1; k <= max_k; ++k) {
                sum += (seq_t)30 * cache[i - k];
            }

            /* subtract 465 * a[i-44] when i >= 44 */
            if (i >= 44) {
                sum -= (seq_t)465 * cache[i - 44];
            }

            /* add the numerator term N(i) */
            seq_t n_term;
            if (i == 0 || i == 44) {
                n_term = (seq_t)1;
            } else if (i <= 43) {
                n_term = (seq_t)2;
            } else {
                n_term = (seq_t)0;
            }

            cache[i] = sum + n_term;
        }

        max_computed = (size_t)n;
    }

    return cache[n];
}
