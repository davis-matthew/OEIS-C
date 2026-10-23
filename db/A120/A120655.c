/*
 * A120655  Expansion of (1-x)*(1+8*x+60*x^2)/((1-2*x)*(1+2*x)*(1-4*x)).
 * https://oeis.org/A120655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(0) = 1; for n > 0, a(n) = 3*(-2)^n - 5*2^n + 27*4^(n-1)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* 2^n */
    seq_t pow2n = pow_seq((seq_t)2, n);

    /* 4^(n-1) (valid for n >= 1 here) */
    seq_t pow4 = pow_seq((seq_t)4, n - 1);

    /* 27 * 4^(n-1) */
    seq_t term = ((seq_t)27) * pow4;

    /* Subtract (2 or 8) * 2^n depending on parity of n */
    seq_t sub = (n & 1) ? (8 * pow2n) : (2 * pow2n);

    return term - sub;
}
