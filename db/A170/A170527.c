/*
 * A170527  Number of reduced words of length n in Coxeter group on 38
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^46 = I.
 * https://oeis.org/A170527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS Sequence A170527 
   Recurrence derived from the given generating function:

   For n = 0 :  a(0) = 1 

   For 1 ≤ n ≤ 45 :  
       a(n) = 36 * Σ_{i=1}^{n} a(n-i) + 2

   For n ≥ 46 :  
       a(n) = 36 * Σ_{i=1}^{45} a(n-i) + 666 * a(n-46)

   The sequence is memoised so that calls with arbitrary
   values of `n` are handled efficiently.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* enlarge cache if necessary */
    if (n >= cache_len) {
        /* Determine new size */
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – return 0 (fallback) */
            return (seq_t)0;
        }
        cache = tmp;

        /* Compute missing terms */
        for (idx_t k = cache_len; k <= n; ++k) {
            seq_t sum = 0;
            /* Sum previous terms */
            for (idx_t i = 1; i <= 45 && k >= i; ++i) {
                sum += cache[k - i];
            }
            seq_t val = sum * (seq_t)36;
            if (k <= 45) {          /* early terms */
                if (k == 0)
                    val += (seq_t)1;
                else
                    val += (seq_t)2;
            } else {                /* terms from 46 onward */
                val += (seq_t)666 * cache[k - 46];
            }
            cache[k] = val;
        }
        cache_len = new_len;
    }

    return cache[n];
}
