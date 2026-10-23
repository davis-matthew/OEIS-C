/*
 * A016137  Expansion of 1/((1-3*x)*(1-6*x)).
 * https://oeis.org/A016137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*6^n - 3^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    seq_t pow6 = pow_int((seq_t)6, n);
    seq_t pow3 = pow_int((seq_t)3, n);
    return pow6 * (seq_t)2 - pow3;
}
