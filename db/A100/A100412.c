/*
 * A100412  a(n) = 8*10^n - 7.
 * https://oeis.org/A100412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 8*10^n - 7
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t tenpow = pow10(n);
    seq_t result = tenpow * (seq_t)8;
    result -= (seq_t)7;
    return result;
}
