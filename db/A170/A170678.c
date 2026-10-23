/*
 * A170678  Number of reduced words of length n in Coxeter group on 45
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^49 = I.
 * https://oeis.org/A170678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n≥1, a(n) = p(n) + 43 × (a(n−1)+…+a(n−48)) – 946 ×
 *            a(n−49), where p(0)=p(49)=1 and p(n)=2 for 1≤n≤48
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;  /* array storing computed values      */
    static idx_t next     = 0;     /* first index not computed yet      */

    if (next <= n) {
        /* grow storage to hold 0..n */
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        cache = tmp;

        /* compute missing terms */
        for (idx_t i = next; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k <= 48 && i >= k; ++k) {
                sum += cache[(size_t)(i - k)];
            }

            seq_t p = (i == 0 || i == 49) ? (seq_t)1 : (seq_t)2;

            if (i >= 49) {
                cache[(size_t)i] = p + ((seq_t)43 * sum) - ((seq_t)946 * cache[(size_t)(i - 49)]);
            } else {
                cache[(size_t)i] = p + ((seq_t)43 * sum);
            }
        }
        next = n + 1;
    }

    return cache[(size_t)n];
}
