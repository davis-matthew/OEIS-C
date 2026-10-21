/*
 * A055853  Convolution of A055852 with A011782.
 * https://oeis.org/A055853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 19, u64 n <= 45, u128 n <= 103,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A055853
 *  a(n) = coefficient of x^n in x*(1-x)^6/(1-2x)^7
 *  Equivalently, a(n)=T(n,6)=A055587(n+6,7).
 *  Computed via the explicit finite sum:
 *  a(n) = Σ_{j=0}^{min(6,n-1)} (-1)^j * binom(6,j) *
 *         binom(n-1-j+6,6) * 2^(n-1-j)
 */
#include <stddef.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

static const idx_t BINOM6[7] = {1, 6, 15, 20, 15, 6, 1};

seq_t a(idx_t n)
{
    /* a(0) = 0 */
    if (n == 0)
        return (seq_t)0;

    seq_t sum_even = 0;
    seq_t sum_odd  = 0;

    /* j runs from 0 to 6, but not beyond n-1 */
    for (idx_t j = 0; j <= 6 && j <= n - 1; ++j) {
        idx_t m = n - 1 - j;                /* exponent of 2 and offset in binomial */
        seq_t term = binomial(m + 6, 6);    /* binom(n-1-j+6,6) */
        term *= pow2(m);                    /* multiply by 2^m */

        /* multiply by binom(6,j) and add to even or odd accumulator */
        if ((j & 1U) == 0)                 /* even j -> positive sign */
            sum_even += term * (seq_t)BINOM6[j];
        else                               /* odd j -> negative sign */
            sum_odd  += term * (seq_t)BINOM6[j];
    }

    /* final result = sum_even - sum_odd; result is guaranteed non‑negative */
    return sum_even - sum_odd;
}
