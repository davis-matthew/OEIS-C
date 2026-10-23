/*
 * A176633  a(n) = 6*a(n-1)-8*a(n-2) for n > 2; a(0) = 83, a(1) = 708, a(2) =
 *          2952.
 * https://oeis.org/A176633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 83, a(n) = 6*(32*4^n-5*2^n) for n > 0
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 12, u64 n <= 28, u128 n <= 60,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"   /* provides big_pow(seq_t, seq_t) */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)83;
    }

    seq_t four_n = big_pow((seq_t)4, (seq_t)n);   /* 4^n */
    seq_t two_n  = big_pow((seq_t)2, (seq_t)n);   /* 2^n */

    seq_t term = (seq_t)32 * four_n - (seq_t)5 * two_n;
    term *= (seq_t)6;

    return term;
}
