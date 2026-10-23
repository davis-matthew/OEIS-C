/*
 * A169218  Number of reduced words of length n in Coxeter group on 13
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^28 = I.
 * https://oeis.org/A169218
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

/* Sequence A169218: number of reduced words of length n in the Coxeter
 * group on 13 generators S_i with relations (S_i)^2 = (S_i S_j)^28 = 1.
 *
 * The generating function is a rational function which gives the linear
 * recurrence
 *
 *     a(n) - 11*∑_{k=1}^{27} a(n-k) + 66*a(n-28) = π(n)
 *
 * where
 *
 *     π(n) = 1  for n = 0 or n = 28,
 *     π(n) = 2  for 1 ≤ n ≤ 27,
 *     π(n) = 0  for n > 28.
 *
 * This recurrence is used to compute all terms.  The function maintains a
 * static cache so that any number of calls to a(idx_t n) in any order
 * runs in O(n) time and O(n) space.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of computed terms */

    if (n >= cache_len) {
        /* grow the cache to hold terms up to n */
        cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t sum_prev = 0;
            /* sum of the previous 27 terms (or fewer if i<27) */
            for (idx_t k = 1; k <= 27 && k <= i; ++k)
                sum_prev += cache[i - k];

            seq_t term = 0;
            if (i >= 28)
                term += (seq_t)66 * cache[i - 28];

            seq_t pi = 0;
            if (i == 0 || i == 28)
                pi = 1;
            else if (i <= 27)
                pi = 2;

            seq_t val = pi + (seq_t)11 * sum_prev - term;
            cache[i] = val;
        }
        cache_len = n + 1;
    }
    return cache[n];
}
