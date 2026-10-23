/*
 * A170587  Number of reduced words of length n in Coxeter group on 50
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^47 = I.
 * https://oeis.org/A170587
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

/* Sequence A170587: number of reduced words of length n in the Coxeter
   group on 50 generators S_i with S_i^2 = I and (S_i S_j)^47 = I.
   The growth series satisfies
        a_k - 48 a_{k-1} - ... - 48 a_{k-46} + 1176 a_{k-47} = RHS_k
   where RHS_k = { 1 if k=0 or 47, 2 if 1 ≤ k ≤ 46, 0 otherwise }.
   Hence
        a_k = 48( a_{k-1} + ... + a_{k-46} )
             - 1176 a_{k-47} + RHS_k            (a_{negative}=0).
   This function memoises the values so that calls can be made in any
   order.  See A170587 for the first 15 terms. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t max_cached = 0;  /* highest index that has been computed */

    /* initialise cache if first call */
    if (cache == NULL) {
        cache = (seq_t *)calloc(1, sizeof(seq_t));   /* at least one element */
        if (!cache) abort();
        cache[0] = (seq_t)1;
    }

    /* extend cache up to n if necessary */
    if (n > max_cached) {
        /* enlarge the array */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;

        /* compute values from max_cached+1 up to n */
        for (idx_t i = max_cached + 1; i <= n; ++i) {
            seq_t rhs;
            if (i == 47)          rhs = (seq_t)1;
            else if (i <= 46)     rhs = (seq_t)2;
            else                  rhs = (seq_t)0;   /* i > 47 */

            /* sum of the previous up to 46 terms (or fewer if i<46) */
            seq_t sum = (seq_t)0;
            idx_t m = (i < 47) ? i : 46;
            for (idx_t k = 1; k <= m; ++k)
                sum += cache[i - k];

            /* apply recurrence */
            seq_t val = rhs + (seq_t)48 * sum;
            if (i >= 47)
                val -= (seq_t)1176 * cache[i - 47];

            cache[i] = val;
        }

        max_cached = n;
    }

    /* return the requested value */
    return cache[n];
}
