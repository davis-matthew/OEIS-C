/*
 * A169000  Number of reduced words of length n in Coxeter group on 35
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^23 = I.
 * https://oeis.org/A169000
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   grow_seq_array, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A169000: number of reduced words of length n
   in the Coxeter group on 35 generators
   with relations (S_i)^2 = (S_i S_j)^23 = I.
   The recurrence follows from the rational generating function

        N(t) = 1 + 2t + 2t^2 + … + 2t^22 + t^23
        D(t) = 1 – 33t – 33t^2 – … – 33t^22 + 561t^23

   For n ≥ 23:
       a(n) = 33·Σ_{k=1}^{22} a(n-k) – 561·a(n-23) + 1
   For 0 < n < 23:
       a(n) = 33·Σ_{k=1}^{n} a(n-k) + 2
   For n = 0:
       a(0) = 1
*/

#include <stdbool.h>
#include "../utils/mul_small.h"
#include "../utils/grow_seq_array.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static idx_t  cached_max = (idx_t)-1;   /* sentinel for “no terms yet” */

    /* ensure capacity */
    if ((size_t)n + 1 > cache_cap)
        grow_seq_array(&cache, &cache_cap, (size_t)n + 1);

    /* compute missing terms up to n */
    if (cached_max == (idx_t)-1 || n > cached_max) {
        idx_t start;
        /* initialise a(0) when cache is empty */
        if (cached_max == (idx_t)-1) {
            cache[0] = (seq_t)1;
            start    = 1;
            cached_max = 0;
        } else
            start = cached_max + 1;

        for (idx_t i = start; i <= n; ++i) {
            seq_t sum = 0;
            idx_t limit = (i < 22) ? i : (idx_t)22;
            for (idx_t j = 1; j <= limit; ++j)
                sum += cache[i - j];

            seq_t term = mul_small(sum, 33U);

            if (i >= 23)
                term -= mul_small(cache[i - 23], 561U);

            /* add the constant term */
            if (i == 0)
                term = (seq_t)1;          /* a(0) */
            else if (i < 23)
                term += (seq_t)2;         /* for 1 ≤ n ≤ 22 */
            else
                term += (seq_t)1;         /* for n ≥ 23 */

            cache[i] = term;
        }
        if (n > cached_max)
            cached_max = n;
    }

    return cache[n];
}
