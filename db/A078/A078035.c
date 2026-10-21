/*
 * A078035  Expansion of (1-x)/(1+2*x^2-x^3).
 * https://oeis.org/A078035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 12, i16 n <= 26, i32 n <= 43, i64 n <= 43, i128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A078035: a(n) is the coefficient of xⁿ in (1‑x)/(1+2x²‑x³).
   The series has the initial values
        a(0)= 1,  a(1)=‑1,  a(2)=‑2,  a(3)= 3
   and satisfies the recurrence for n≥4
        a(n) = –2·a(n‑2) + a(n‑3) .
   This recurrence follows from (1+2x²‑x³)·∑a(n)xⁿ = 1‑x.  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* dynamic array of computed terms    */
    static idx_t cache_len = 0;   /* number of cached terms (size of array) */

    /* If the cache is too small, grow it to accommodate index n.  */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        /* Compute any new terms that were not previously cached.  */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0)          cache[i] = (seq_t) 1;
            else if (i == 1)     cache[i] = (seq_t)-1;
            else if (i == 2)     cache[i] = (seq_t)-2;
            else if (i == 3)     cache[i] = (seq_t) 3;
            else                 cache[i] = ((seq_t)-2) * cache[i-2] + cache[i-3];
        }
        cache_len = new_len;
    }
    return cache[n];
}
