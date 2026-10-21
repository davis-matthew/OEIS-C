/*
 * A066415  Denominators of coefficients in series expansion of
 *          -512*(1+x)^3/(x-8)^3.
 * https://oeis.org/A066415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Coefficients of -512*(1+x)^3/(x-8)^3 are rational numbers.
   Let
        N_n = Σ_{i=0}^{min(3,n)} C(3,i) * C(n-i+2,2) * 8^i .
   The coefficient a_n is N_n / 8^n, reduced to lowest terms.
   Thus the denominator equals 8^n / 2^{v_2(N_n)} = 2^{3*n - v_2(N_n)}.
   This function returns that denominator as a power of two. */
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* binomial coefficients C(3,i) for i=0..3 */
    const seq_t binom3[4] = {1, 3, 3, 1};
    /* powers of 8^i for i=0..3 */
    const seq_t pow8[4] = {1, 8, 64, 512};

    idx_t limit = (n < 3ULL) ? n : 3ULL;
    seq_t N = 0;

    for (idx_t i = 0; i <= limit; ++i) {
        seq_t k = n - i;                           /* k = n - i >= 0                */
        seq_t binom = ((seq_t)(k + 2) * (seq_t)(k + 1)) / 2;   /* C(k+2,2) */
        seq_t term = binom3[i] * binom;             /* C(3,i)*C(k+2,2)              */
        term *= pow8[i];                            /* multiply by 8^i               */
        N += term;
    }

    /* count trailing zeros of N (the 2-adic valuation v₂(N)) */
    seq_t temp = N;
    idx_t v = 0;
    while ((temp & 1) == 0) {
        temp >>= 1;
        ++v;
    }

    /* exponent of two in the reduced denominator: e = 3*n - v */
    idx_t exp = 3 * n - v;
    return pow2(exp);  /* 2^exp */
}
