/*
 * A170726  Number of reduced words of length n in Coxeter group on 45
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^50 = I.
 * https://oeis.org/A170726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A170726:
   a(0) = 1
   For n ≥ 1:
      a(n) = c(n) + 43 * Σ_{k=1}^{min(n,49)} a(n−k)  –  (n ≥ 50 ? 946 * a(n−50) : 0)
   where c(n) = 1   if n = 50,
            2   if 1 ≤ n ≤ 49,
            0   otherwise.
*/

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    size_t need_len = (size_t)n + 1;

    /* enlarge cache if necessary */
    if (need_len > cache_len) {
        seq_t *new_cache = (seq_t *)realloc(cache, need_len * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;     /* allocation failure */
        cache = new_cache;

        /* compute missing entries */
        for (size_t i = cache_len; i < need_len; ++i) {
            idx_t ii = (idx_t)i;
            if (ii == 0) {        /* a(0) = 1 */
                cache[0] = (seq_t)1;
                continue;
            }

            /* coefficient from the numerator */
            seq_t coef = 0;
            if (ii == 50)
                coef = (seq_t)1;
            else if (ii < 50)
                coef = (seq_t)2;

            /* partial sum of the last up to 49 terms */
            size_t maxk = ii < 49 ? ii : 49;
            seq_t sum = 0;
            for (idx_t k = 1; k <= maxk; ++k)
                sum += cache[i - k];

            /* recurrence */
            seq_t val = coef + (seq_t)43 * sum;
            if (ii >= 50)
                val -= (seq_t)946 * cache[i - 50];

            cache[i] = val;
        }
        cache_len = need_len;
    }
    return cache[(size_t)n];
}
