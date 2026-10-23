/*
 * A170043  Number of reduced words of length n in Coxeter group on 34
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^36 = I.
 * https://oeis.org/A170043
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A170043:
   Let coeff(n) = 1 if n==0, 2 if 1≤n≤35, 1 if n==36, 0 otherwise.
   For n≥1, a(n) = coeff(n) + 32·∑_{i=1}^{35} a(n−i) – 528·a(n−36),
   with a(0)=1.  This is obtained from the rational generating
   function N(t)=t^36+2t^35+…+2t+1 divided by D(t)=1−32t−…−32t^35+528t^36. */
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (cache_size <= n) {
        size_t new_size = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) return 0;                 /* allocation failed */
        cache = new_cache;

        for (idx_t k = cache_size; k <= n; ++k) {
            seq_t term;
            /* numerator coefficient */
            if (k == 0)
                term = 1;
            else if (k <= 35)
                term = 2;
            else if (k == 36)
                term = 1;
            else
                term = 0;

            /* add contributions from a(k-i) */
            idx_t up = k < 35 ? k : 35;
            for (idx_t i = 1; i <= up; ++i)
                term += (seq_t)32 * cache[k - i];

            /* subtract 528 * a(k-36) if applicable */
            if (k >= 36)
                term -= (seq_t)528 * cache[k - 36];

            cache[k] = term;
        }
        cache_size = new_size;
    }

    return cache[n];
}
