/*
 * A170389  Number of reduced words of length n in Coxeter group on 44
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^43 = I.
 * https://oeis.org/A170389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n) satisfies the linear recurrence derived from
 * its generating function:
 *  a(0) = 1;
 *  for 1 ≤ n ≤ 42:  a(n) = 42·a(n-1) + 42·a(n-2) + … + 42·a(n-42) + 2;
 *  a(43) = 42·a(42) + 42·a(41) + … + 42·a(1) – 903·a(0) + 1;
 *  for n ≥ 44:  a(n) = 42·a(n-1) + 42·a(n-2) + … + 42·a(n-42) – 903·a(n-43). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (cache_size <= n) {
        idx_t old_size = cache_size;
        cache_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
        if (!tmp) abort();          /* memory allocation failure */
        cache = tmp;

        for (idx_t i = old_size; i <= n; ++i) {
            seq_t sum = 0;
            /* add 42*a(i-k) for k = 1..42 when available */
            for (idx_t k = 1; k <= 42; ++k) {
                if (i >= k) {
                    sum += (seq_t)42 * cache[i - k];
                }
            }
            /* subtract 903*a(i-43) if i ≥ 43 */
            if (i >= 43) {
                sum -= (seq_t)903 * cache[i - 43];
            }

            /* right‑hand side coefficient from the numerator polynomial */
            seq_t rhs;
            if (i == 0) {
                rhs = (seq_t)1;
            } else if (i <= 42) {
                rhs = (seq_t)2;
            } else if (i == 43) {
                rhs = (seq_t)1;
            } else {
                rhs = (seq_t)0;
            }

            cache[i] = sum + rhs;
        }
    }
    return cache[n];
}
