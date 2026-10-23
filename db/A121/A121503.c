/*
 * A121503  Numerators of partial sums of a series for sqrt(2) + sqrt(3)
 *          involving Catalan numbers.
 * https://oeis.org/A121503
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = numerator of r(n) where r(n) = 4 - ( Σ_{k=0..n}
 *            C(k)*(1+2^(k+1))/16^k )/4 C(k) = Catalan number Algorithm:
 *            compute 16^n, then the sum with a common denominator 16^n, form
 *            the numerator N = 16*16^n - sum, and divide out all factors of 2
 *            up to 2^(4n+2) (the denominator) to obtain the reduced...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* 16^n */
    seq_t pow16_n = 1;
    for (idx_t i = 0; i < n; ++i)
        pow16_n <<= 4;           /* multiply by 16 */

    seq_t pow16_div = pow16_n;   /* will be divided by 16 each step */
    seq_t cat       = 1;         /* C(0) */
    seq_t pow2      = 2;         /* 2^(0+1) */
    seq_t numerator_S = 0;       /* Σ C(k)*(1+2^(k+1))*16^(n-k) */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = cat * (pow2 + 1);   /* C(k)*(1+2^(k+1)) */
        term *= pow16_div;               /* *16^(n-k)  */
        numerator_S += term;             /* accumulate */

        if (k < n) {                     /* prepare next Catalan and 2^ */
            cat = cat * (seq_t)(4 * k + 2);
            cat /= (k + 2);               /* integer division exact */
            pow2 <<= 1;                   /* double 2^(k+1) -> 2^(k+2) */
        }

        pow16_div >>= 4;                  /* divide by 16 for next k */
    }

    /* Numerator N = 16*16^n - Σ ...  */
    seq_t N = (pow16_n << 4) - numerator_S;

    /* Remove common factor of 2 (denominator is 2^(4n+2)) */
    seq_t result = N;
    idx_t    exp  = 4 * n + 2;            /* exponent of 2 in denominator */
    while ((result & 1) == 0 && exp > 0) {
        result >>= 1;
        --exp;
    }

    return result;
}
