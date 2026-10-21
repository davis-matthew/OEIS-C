/*
 * A086090  a(n) = 2^n + n*3^n.
 * https://oeis.org/A086090
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(n) = 2^n + n*3^n
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   pow_int_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int_seq.h"

seq_t a(idx_t n)
{
    seq_t pow2 = pow_int_seq((seq_t)2, n);   /* 2^n */
    seq_t pow3 = pow_int_seq((seq_t)3, n);   /* 3^n */
    seq_t res  = pow2 + (seq_t)n * pow3;     /* 2^n + n*3^n */
    return res;
}
