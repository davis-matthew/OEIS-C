/*
 * A170034  Number of reduced words of length n in Coxeter group on 25
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^36 = I.
 * https://oeis.org/A170034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A170034: Number of reduced words of length n in the Coxeter group
   on 25 generators with relations S_i^2 = (S_i S_j)^36 = I.
   The generating function yields a linear recurrence of order 36:
   for 0 ≤ n ≤ 35:
       a(0) = 1
       a(n) = 2 + 23 * Σ_{i=1}^{n} a(n-i)
   for n = 36:
       a(36) = 1 + 23 * Σ_{i=1}^{35} a(36-i) – 276 * a(0)
   for n ≥ 37:
       a(n) = 23 * Σ_{i=1}^{35} a(n-i) – 276 * a(n-36)
   First terms: 1,25,600,14400,345600,...
*/

#include <stdlib.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"

/* cache for computed values */
static seq_t *seq_cache = NULL;
static size_t  seq_cap  = 0;       /* current capacity of seq_cache      */
static idx_t   seq_last = (idx_t)(-1); /* last index for which a() has been computed */

/* Compute a(n), using memoisation and the recurrence above */
seq_t a(idx_t n)
{
    /* Allocate or grow the cache so that indices 0 … n are available */
    if (n >= seq_cap)
    {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = NULL;

        if (seq_cache == NULL)
        {   /* first allocation */
            new_cache = seq_alloc_array(new_cap);
        }
        else
        {   /* grow: allocate a larger array, copy old values, free old array */
            new_cache = seq_alloc_array(new_cap);
            for (size_t i = 0; i < seq_cap; ++i)
                new_cache[i] = seq_cache[i];
            seq_free_array(seq_cache);
        }

        seq_cache = new_cache;
        seq_cap   = new_cap;
    }

    /* Compute missing terms sequentially (O(n)) */
    for (idx_t k = seq_last + 1; k <= n; ++k)
    {
        if (k == 0)
        {
            seq_cache[0] = (seq_t)1;
        }
        else if (k <= 35)
        {
            seq_t sum = 0;
            for (idx_t i = 1; i <= k; ++i)
                sum += seq_cache[k - i];
            seq_cache[k] = (seq_t)2 + (seq_t)23 * sum;
        }
        else if (k == 36)
        {
            seq_t sum = 0;
            for (idx_t i = 1; i <= 35; ++i)
                sum += seq_cache[36 - i];
            seq_cache[36] = (seq_t)1 + (seq_t)23 * sum - (seq_t)276 * seq_cache[0];
        }
        else   /* k ≥ 37 */
        {
            seq_t sum = 0;
            for (idx_t i = 1; i <= 35; ++i)
                sum += seq_cache[k - i];
            seq_cache[k] = (seq_t)23 * sum - (seq_t)276 * seq_cache[k - 36];
        }
    }

    seq_last = n;
    return seq_cache[n];
}
