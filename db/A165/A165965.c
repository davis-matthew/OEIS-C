/*
 * A165965  Number of reduced words of length n in Coxeter group on 24
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^10 = I.
 * https://oeis.org/A165965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   driver
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A165965: number of reduced words of length n in the
   Coxeter group with 24 generators S_i and relations (S_i)^2 = (S_i S_j)^10 = I.

   Generating function (g.f.):  
      P(t) / Q(t)  where  
      P(t) = t^10 + 2*t^9 + 2*t^8 + 2*t^7 + 2*t^6 + 2*t^5 + 2*t^4 + 2*t^3 + 2*t^2 + 2*t + 1  
      Q(t) = 253*t^10 - 22*t^9 - 22*t^8 - 22*t^7 - 22*t^6 - 22*t^5 - 22*t^4 - 22*t^3 
             - 22*t^2 - 22*t + 1

   From  Q(t) * G(t) = P(t)  we obtain the linear recurrence
      a_n - 22(a_{n-1} + … + a_{n-9}) + 253 a_{n-10} = d_n          (n ≥ 0)

   with the initial “driver” values  
      d_0   = 1
      d_n   = 2   for 1 ≤ n ≤ 9
      d_{10} = 1
      d_n   = 0   for n ≥ 11

   Rearranged
      a_n = d_n + 22 * Σ_{k=1}^{9} a_{n-k} - 253 * a_{n-10}   (a_j = 0 for j<0).
   This recurrence can be evaluated in O(n) time with small constant.
*/

#include <stdlib.h>
#include "../utils/driver.h"

static seq_t *cache = NULL;              /* cached values a(0)..a(max)   */
static size_t cached_size = 0;            /* number of stored terms        */

seq_t a(idx_t n)
{
    /* initialise cache on first use */
    if (cache == NULL) {
        cache      = (seq_t*)malloc(sizeof(seq_t));
        if (cache == NULL) return 0;   /* allocation failed, return 0 */
        cached_size = 0;
    }

    /* extend cache sufficient for index n */
    if ((idx_t)n >= cached_size) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0; /* reallocation failed */
        cache = tmp;

        /* compute missing terms */
        for (idx_t i = cached_size; i <= n; ++i) {
            seq_t sum = 0;
            /* sum of last at most 9 terms */
            for (idx_t k = 1; k <= 9 && k <= i; ++k)
                sum += cache[i - k];

            seq_t term = driver(i) + (seq_t)22 * sum;
            if (i >= 10) term -= (seq_t)253 * cache[i - 10];

            cache[i] = term;
        }
        cached_size = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
