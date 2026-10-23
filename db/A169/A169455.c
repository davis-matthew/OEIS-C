/*
 * A169455  Number of reduced words of length n in Coxeter group on 10
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^33 = I.
 * https://oeis.org/A169455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the recurrence a(0)=1,
 *            a(n)=2+8⋅∑_{i=1}^{min(n,32)} a(n-i) for 1≤n≤32,
 *            a(33)=1+8⋅∑_{i=1}^{32} a(33-i)−36⋅a(0), a(n)=8⋅∑_{i=1}^{32}
 *            a(n-i)−36⋅a(n-33) for n≥34
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t next_computed = 0;

    /* initialise cache with a(0)=1 */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;
        cache[0] = (seq_t)1;
        next_computed = 1;
    }

    /* extend cache if necessary */
    if (n >= next_computed) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;          /* allocation failure -> 0 */
        cache = tmp;

        for (idx_t k = next_computed; k <= n; ++k) {
            idx_t i_max = (k < 32 ? k : (idx_t)32);
            seq_t sum = 0;
            for (idx_t i = 1; i <= i_max; ++i) sum += cache[k - i];

            seq_t term1 = sum;                     /* 8*sum will be done below */
            term1 *= (seq_t)8;

            seq_t term2 = 0;
            if (k >= 33) {                         /* -36*a[k-33] term */
                term2 = cache[k - 33];
                term2 *= (seq_t)36;
            }

            seq_t const_term = 0;
            if (k >= 1 && k <= 32) {
                const_term = (seq_t)2;
            } else if (k == 33) {
                const_term = (seq_t)1;
            }

            cache[k] = term1 - term2 + const_term;
        }
        next_computed = n + 1;
    }

    return cache[n];
}
