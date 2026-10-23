/*
 * A046662  Sum of mistyped version of binomial coefficients.
 * https://oeis.org/A046662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A046662: Sum of mistyped binomial coefficients
   Formula (recurrence form):
       a(0) = 1
       a(1) = 2
       a(n) = n^2 * a(n-1) - n*(n-1) * a(n-2) + 1   for n >= 2
   This recurrence gives the same values as
       a(n) = Σ_{k=0..n} n! * k! / (n-k)!
   The implementation memoizes previous terms so that arbitrary
   calls in any order are O(1) after initializations.
*/

#include <stdlib.h>  /* malloc, realloc */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* memoised values */
    static idx_t  cached_size = 0;  /* one past last valid index */

    /* first allocation: terms a(0) and a(1) */
    if (cached_size == 0) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!cache) return (seq_t)0;      /* out‑of‑memory guard */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cached_size = 2;
    }

    /* extend cache if required */
    if (n >= cached_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;       /* out‑of‑memory guard */
        cache = tmp;

        /* compute missing terms using the recurrence */
        for (idx_t i = cached_size; i <= n; ++i) {
            seq_t prev1 = cache[i-1];
            seq_t prev2 = cache[i-2];
            seq_t nn = (seq_t)i;
            seq_t term = nn*nn*prev1 - nn*(nn-1)*prev2 + (seq_t)1;
            cache[i] = term;
        }
        cached_size = (idx_t)(n + 1);
    }

    return cache[n];
}
