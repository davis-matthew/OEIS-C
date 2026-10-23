/*
 * A169407  Number of reduced words of length n in Coxeter group on 10
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^32 = I.
 * https://oeis.org/A169407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   N_coeff
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A169407
   a(0) = 1
   For n ≥ 1:
     let S(n) = Σ_{k=1}^{min(31,n)} a(n−k)
     let T(n) = (n ≥ 32 ? a(n−32) : 0)
     let N(n) = 1          if n = 0 or n = 32
                 2          if 1 ≤ n ≤ 31
                 0          otherwise
     then a(n) = 8·S(n) − 36·T(n) + N(n)
*/

#include <stdlib.h>
#include <stdint.h>
#include "../utils/N_coeff.h"

static seq_t *cache = NULL;     /* cache of computed terms */
static idx_t  cache_size = 0;   /* number of terms stored */
static size_t cache_cap  = 0;   /* allocated capacity */

/* Return the nth term of the sequence */
seq_t a(idx_t n)
{
    /* Ensure the cache holds at least n+1 terms */
    if (cache == NULL) {
        /* start with a[0] = 1 */
        cache_cap   = 1;
        cache       = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0]    = 1;
        cache_size  = 1;
    }

    if (n >= cache_size) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap > cache_cap) {
            /* grow the cache */
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) { /* allocation failure – let the caller fail */
                exit(1);
            }
            cache     = tmp;
            cache_cap = new_cap;
        }
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t sum31 = 0;
            /* sum over the last up to 31 terms */
            for (uint32_t k = 1; k <= 31 && i >= k; ++k) {
                sum31 += cache[(size_t)(i - k)];
            }
            seq_t term32 = 0;
            if (i >= 32) term32 = cache[(size_t)(i - 32)];
            seq_t ai = (sum31 * 8) - (term32 * 36) + N_coeff(i);
            cache[(size_t)i] = ai;
        }
        cache_size = n + 1;
    }
    return cache[(size_t)n];
}
