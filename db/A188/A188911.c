/*
 * A188911  Binomial convolution of the binomial coefficients bin(3n,n)
 *          (A005809).
 * https://oeis.org/A188911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * binomial(3*k,k) *
 *            binomial(3*n-3*k,n-k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 35,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 86 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Uses the helper function binom from utils/binom.h) */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 is handled automatically by the loop */
    seq_t result = 0;

    for (idx_t k = 0; k <= n; ++k)
    {
        /* Compute the three binomial coefficients */
        seq_t C   = binomial(n, k);
        seq_t t1  = binomial(3 * k, k);
        seq_t t2  = binomial(3 * (n - k), n - k);

        /* term = C * t1 * t2 */
        seq_t term = C * t1;
        term = term * t2;

        /* Add to result */
        result += term;
    }

    return result;
}
