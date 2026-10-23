/*
 * A167115  Number of reduced words of length n in Coxeter group on 14
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^14 = I.
 * https://oeis.org/A167115
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A167115: Number of reduced words of length n in
   the Coxeter group with 14 generators S_i, S_i^2 = I and
   (S_i S_j)^14 = I.
   The values satisfy the linear recurrence derived from the
   generating function
          a(0) = 1
     a(n) = 12*a(n-1) + 12*a(n-2) + … + 12*a(n-13)
            – 78*a(n-14) + c_n
   where
          c_n = 2   for 1 ≤ n ≤ 13
          c_n = 1   for n = 0 or 14
          c_n = 0   for n ≥ 15
   (All other terms are zero automatically because the
   recurrence uses only previous values.) *
   The routine memoises already-computed values so that
   it runs in O(n) time and is safe for any order of calls. */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;        /* number of stored elements */

    if (n >= cache_size) {
        /* enlarge cache to hold indices up to n */
        size_t new_size = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) {
            /* allocation failed – return 0 in this unlikely case */
            return 0;
        }

        /* compute values from current size up to n */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t rhs;
            if (i == 0)
                rhs = 1;
            else if (i <= 13)
                rhs = 2;
            else if (i == 14)
                rhs = 1;
            else
                rhs = 0;

            seq_t sum = 0;
            /* add 12 * a(i-k) for k = 1 .. 13, if i >= k */
            for (idx_t k = 1; k <= 13; ++k) {
                if (i >= k) {
                    sum += new_cache[i - k] * 12;
                }
            }
            /* subtract 78 * a(i-14) if applicable */
            if (i >= 14) {
                sum -= new_cache[i - 14] * 78;
            }
            sum += rhs;
            new_cache[i] = sum;
        }

        cache = new_cache;
        cache_size = new_size;
    }

    return cache[n];
}
