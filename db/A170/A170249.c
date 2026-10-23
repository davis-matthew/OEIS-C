/*
 * A170249  Number of reduced words of length n in Coxeter group on 48
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^40 = I.
 * https://oeis.org/A170249
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

/* Recurrence for A170249:
   Let rhs(n) = 1 for n = 0 or 40,
                2 for 1 ≤ n ≤ 39,
                0 otherwise.
   For n ≥ 0:
   a(n) = 46 * sum_{i=1}^{min(39,n)} a(n-i)
          - 1081 * a(n-40)   (term is 0 if n < 40)
          + rhs(n)

   The sequence is stored in a static cache, so
   calls to a(n) can arrive in any order.
*/
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* cache[0 .. cache_len-1] : all computed a(k) */
    static idx_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Grow cache if necessary */
    if (cache_len <= n) {
        size_t needed = (size_t)(n + 1);
        if (cache_cap < needed) {
            size_t new_cap = cache_cap ? cache_cap : 1;
            while (new_cap < needed) new_cap <<= 1;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }

        /* Sum of the last up to 39 terms (used for the 46-factor) */
        seq_t sum_last = 0;
        /* For positions already computed before this expansion,
           rebuild sum_last from the tail of the cache. */
        idx_t start = 0;
        if (cache_len > 0) {
            /* Recompute sum_last for the last min(39, cache_len-1) terms */
            sum_last = 0;
            idx_t last = cache_len - 1;
            idx_t from = (last >= 39) ? last - 39 : 0;
            for (idx_t i = from; i <= last; ++i)
                sum_last += cache[i];
        }

        /* If cache was empty, sum_last remains 0. */
        for (idx_t i = cache_len; i <= n; ++i) {
            /* rhs for term i */
            seq_t rhs;
            if (i == 0 || i == 40)
                rhs = (seq_t)1;
            else if (i <= 39 && i >= 1)
                rhs = (seq_t)2;
            else
                rhs = (seq_t)0;

            /* Compute term a(i) */
            seq_t term = (seq_t)46 * sum_last;
            if (i >= 40)
                term -= (seq_t)1081 * cache[i - 40];
            term += rhs;

            cache[i] = term;

            /* Update sum_last for next iteration:
               add current term, subtract a(i-39) if window exceeded 39 */
            sum_last += term;
            if (i >= 39)     /* window now has more than 39 terms */
                sum_last -= cache[i - 39];
        }
        cache_len = n + 1;
    }
    return cache[n];
}
