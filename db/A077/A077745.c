/*
 * A077745  Numerator of integral_{x=1..2} (x^2-1)^n dx.
 * https://oeis.org/A077745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = (2n+1)!/(n! 2^n) * ∫_{x=1}^{2} (x^2-1)^n dx Recurrence
 *            derived from integration by parts: a(0) = 1 a(n) = 2 * D_{n-1} *
 *            3^n – 2n * a(n-1) (n ≥ 1) where D_k = (2k+1)!/(k! 2^k) and D_k+1
 *            = D_k * (2k+3) (k ≥ 0) This computes the numerator of the
 *            required integral
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *a_cache = NULL;          /* a values */
    static seq_t *D_cache = NULL;          /* D values */
    static size_t  cache_len = 0;          /* next index to compute */
    static seq_t pow3 = (seq_t)1;          /* 3^i for current i  */

    /* grow cache if needed */
    if (cache_len <= (size_t)n) {
        /* new size required */
        size_t new_len = (size_t)n + 1;
        a_cache = (seq_t *)realloc(a_cache, new_len * sizeof(seq_t));
        D_cache = (seq_t *)realloc(D_cache, new_len * sizeof(seq_t));

        for (size_t i = cache_len; i < new_len; ++i) {
            if (i == 0) {
                /* initialisation */
                a_cache[0] = (seq_t)1;              /* a(0) = 1 */
                D_cache[0] = (seq_t)1;              /* D_0   = 1 */
                pow3       = (seq_t)1;              /* 3^0   = 1 */
            } else {
                /* update 3^i */
                pow3 *= (seq_t)3;                  /* pow3 = 3^i */

                /* D_i = D_{i-1} * (2i+1) */
                D_cache[i] = D_cache[i-1] * ((seq_t)(2 * i + 1));

                /* a_i = 2 * D_{i-1} * 3^i – 2i * a_{i-1} */
                seq_t term1 = D_cache[i-1] * pow3;
                term1 *= (seq_t)2;
                seq_t term2 = a_cache[i-1] * ((seq_t)i);
                term2 *= (seq_t)2;
                a_cache[i] = term1 - term2;
            }
        }
        cache_len = new_len;
    }
    return a_cache[(size_t)n];
}
