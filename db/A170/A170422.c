/*
 * A170422  Number of reduced words of length n in Coxeter group on 29
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^44 = I.
 * https://oeis.org/A170422
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the generating function:
   a(0) = 1
   For n ≥ 1:
     coef = (n == 44) ? 1 :
            (n <= 43) ? 2 : 0;
     a(n) = coef
            + 27 * Σ_{k=1}^{min(43,n)} a(n-k)
            - 378 * a(n-44)          (if n ≥ 44)
   The RHS vanishes for n > 43 except for the 27‑ and 378‑terms. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initialise cache with a(0) = 1 */
    if (cache_len == 0) {
        cache_len = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (!cache)
            return (seq_t)0;                /* out‑of‑memory: silent failure */
        cache[0] = (seq_t)1;
    }

    /* extend cache if needed */
    if (cache_len <= n) {
        idx_t old_len = cache_len;
        cache_len = n + 1;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_len);
        if (!cache)
            return (seq_t)0;                /* out‑of‑memory: silent failure */

        for (idx_t i = old_len; i <= n; ++i) {
            /* coefficient from the numerator of the generating function */
            seq_t coeff;
            if (i == 44)
                coeff = (seq_t)1;
            else if (i > 0 && i <= 43)
                coeff = (seq_t)2;
            else
                coeff = (seq_t)0;

            seq_t sum = coeff;

            /* add 27 * a[i-k] for k = 1 .. min(43,i) */
            for (idx_t k = 1; k <= 43 && k <= i; ++k) {
                sum += (seq_t)27 * cache[i - k];
            }

            /* subtract 378 * a[i-44] if i ≥ 44 */
            if (i >= 44) {
                sum -= (seq_t)378 * cache[i - 44];
            }

            cache[i] = sum;
        }
    }

    return cache[n];
}
