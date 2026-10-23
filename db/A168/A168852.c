/*
 * A168852  Number of reduced words of length n in Coxeter group on 31
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^20 = I.
 * https://oeis.org/A168852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   ensure_capacity, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: 
   a(0) = 1 ;
   a(n) = 31 * 30^(n-1)          for 1 ≤ n ≤ 19 ;
   a(n) = 15 * a(n-20) - Σ_{k=1}^{19} a(n-k)   for n ≥ 20 .
   The sequence grows very rapidly; values beyond 64‑bit may require
   an arbitrary‑precision build of seq_t.  The function memoises
   previously computed terms so that calls in any order remain fast. */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/seq_pow.h"
#include "../utils/ensure_capacity.h"

/* static cache for memoisation */
static seq_t *a_cache      = NULL;
static size_t a_cache_cap  = 0;
static size_t a_cache_len  = 0;

seq_t a(idx_t n)
{
    /* if we have already computed the value, return it */
    if (n < a_cache_len)
        return a_cache[n];

    /* extend cache up to n */
    size_t need = (size_t)n + 1;
    ensure_capacity(need, &a_cache_cap, (void **)&a_cache, sizeof(seq_t));

    /* compute missing entries */
    for (size_t i = a_cache_len; i <= n; ++i)
    {
        seq_t val;

        if (i == 0)
            val = (seq_t)1;
        else if (i <= 19)
            /* a(i) = 31 * 30^(i-1) */
            val = (seq_t)31 * seq_pow((seq_t)30, (idx_t)(i - 1));
        else
        {
            /* a(i) = 15*a(i-20) - Σ_{k=1}^{19} a(i-k) */
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k <= 19; ++k)
                sum += a_cache[i - k];
            val = (seq_t)15 * a_cache[i - 20] - sum;
        }

        a_cache[i] = val;
    }

    a_cache_len = need;
    return a_cache[n];
}
