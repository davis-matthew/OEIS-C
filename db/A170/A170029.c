/*
 * A170029  Number of reduced words of length n in Coxeter group on 20
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^36 = I.
 * https://oeis.org/A170029
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 18*∑_{i=1}^{35} a(n-i) – 171*a(n-36) + b(n)
   with
      b(0)   = 1,
      1 ≤ b(n) ≤ 35  = 2,
      b(36)  = 1,
      b(n)   = 0 for n > 36.
   This recurrence is obtained from the rational generating function
   of A170029.  Initial values are a(0)=1, a(1)=20, a(2)=380, etc. */

#include <stdlib.h>
#include "../utils/ensure_capacity.h"

static seq_t *cache = NULL;      /* dynamic array of computed terms   */
static idx_t cache_len = 0;      /* number of terms stored           */
static size_t cache_cap = 0;     /* allocated size of the array       */

seq_t a(idx_t n)
{
    /* Extend the cache up to index n */
    while (cache_len <= n) {
        /* Make sure we have space for the next element */
        if (cache_len == cache_cap) {
            /* Ensure capacity for one more element */
            ensure_capacity(cache_len + 1, &cache_cap,
                            (void **)&cache, sizeof(seq_t));
            /* If malloc failed, keep cache_cap equal to the
               actual allocation size returned by ensure_capacity.
               In that unlikely situation the program will abort
               later when accessing an uninitialized element. */
        }

        idx_t k = cache_len;          /* index of the term to compute */

        /* Compute b(k) */
        seq_t b;
        if (k == 0)               b = 1;
        else if (k <= 35)         b = 2;
        else if (k == 36)         b = 1;
        else                      b = 0;

        /* Sum of the last 35 terms */
        seq_t sum = 0;
        if (k > 0) {
            idx_t end = (k >= 35) ? k - 1 : 34;
            /* iterate i = 1 .. min(35, k) */
            for (idx_t i = 1; i <= (k < 35 ? k : 35); ++i) {
                sum += cache[k - i];
            }
        }

        seq_t term = b + 18 * sum;
        if (k >= 36) {
            term -= 171 * cache[k - 36];
        }

        cache[cache_len] = term;
        ++cache_len;
    }

    return cache[n];
}
