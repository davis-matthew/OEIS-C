/*
 * A169067  Number of reduced words of length n in Coxeter group on 6
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^25 = I.
 * https://oeis.org/A169067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula:  a(0) = 1
 *           a(1) = 6
 *           a(n) = 5 * a(n‑1)             for 2 ≤ n ≤ 24
 *           a(25) = 4*(a(24)+⋯+a(1)) – 9
 *           a(n) = 4*(a(n‑1)+⋯+a(n‑24)) – 10*a(n‑25)   for n ≥ 26
 * This recurrence follows from the rational generating function of the
 * sequence.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for computed terms */
    static seq_t *cache   = NULL;   /* array of computed values   */
    static idx_t  size    = 0;      /* number of elements stored  */

    if (n >= size) {
        /* Extend cache */
        size_t new_cap = (size_t)(n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;               /* allocation failed */
        cache = tmp;

        /* Compute new terms */
        for (idx_t i = size; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else if (i == 1) {
                cache[1] = (seq_t)6;
            } else if (i <= 24) {
                cache[i] = (seq_t)5 * cache[i - 1];
            } else if (i == 25) {
                /* a(25) = 4*sum_{i=1}^{24} a(i) - 9  (derived from the generating function) */
                seq_t sum = (seq_t)0;
                for (idx_t j = 1; j <= 24; ++j) sum += cache[i - j];
                cache[i] = (seq_t)4 * sum - (seq_t)9;
            } else { /* i >= 26 */
                /* a(n) = 4*sum_{i=1}^{24} a(n-i) - 10*a(n-25) */
                seq_t sum = (seq_t)0;
                for (idx_t j = 1; j <= 24; ++j) sum += cache[i - j];
                cache[i] = (seq_t)4 * sum - (seq_t)10 * cache[i - 25];
            }
        }
        size = n + 1;
    }
    return cache[n];
}
