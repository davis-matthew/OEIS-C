/*
 * A153336  Number of zig-zag paths from top to bottom of a 2n by 2n square
 *          whose color is that of the top right corner.
 * https://oeis.org/A153336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2n+1)*2^(2n-2) - 2*(2n-1)*binomial(2n-2,n-1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t two_n_plus_one = (seq_t)(2 * n + 1);
    seq_t two_n_minus_one = (seq_t)(2 * n - 1);

    seq_t pow_term = pow2(2 * n - 2);           /* 2^(2n-2) */
    seq_t term1 = two_n_plus_one * pow_term;    /* (2n+1)*2^(2n-2) */

    seq_t binom = binomial(2 * n - 2, n - 1);
    seq_t term2 = 2 * two_n_minus_one * binom;  /* 2*(2n-1)*binomial(...) */

    return term1 - term2;
}
