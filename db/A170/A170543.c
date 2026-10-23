/*
 * A170543  Number of reduced words of length n in Coxeter group on 6
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^47 = I.
 * https://oeis.org/A170543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A170543
   Number of reduced words of length n in Coxeter group on 6 generators S_i
   with relations (S_i)^2 = (S_i S_j)^47 = I.
   The sequence satisfies the linear recurrence
   a(n) = 2 + 4 * Σ_{k=0}^{n-1} a(k)          for 0 ≤ n ≤ 46
   a(n) = (10 * a(n-47) - 4 * Σ_{k=1}^{46} a(n-k)) / 4   for n ≥ 47
 */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache     = NULL;
    static idx_t  cache_cap = 0;
    static idx_t  max_cached = (idx_t)-1;  /* no value cached yet */

    /* Grow cache to hold up to index n (inclusive) */
    if ((idx_t)(n + 1) > cache_cap) {
        size_t new_cap   = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failure – give up */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Initialise first value if not yet done */
    if (max_cached == (idx_t)-1) {
        cache[0]     = (seq_t)1;
        max_cached   = 0;
    }

    /* Compute any missing values up to n */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        if (i <= 46) {   /* recurrence for the first 47 terms */
            seq_t sum = 0;
            for (idx_t k = 0; k < i; ++k)
                sum += cache[k];
            cache[i] = (seq_t)2 + (seq_t)4 * sum;
        } else {        /* recurrence for n ≥ 47 */
            seq_t sum_prev46 = 0;
            for (idx_t k = 1; k <= 46; ++k)
                sum_prev46 += cache[i - k];
            seq_t num = (seq_t)10 * cache[i - 47] - (seq_t)4 * sum_prev46;
            cache[i] = num / (seq_t)4;   /* exact division */
        }
    }

    max_cached = n;
    return cache[n];
}
