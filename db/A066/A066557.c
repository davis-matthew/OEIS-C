/*
 * A066557  Largest n-digit multiple of n.
 * https://oeis.org/A066557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor((10^n-1)/n)*n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* 10^n in arbitrary precision */
    seq_t p = pow_uint(10, n);

    /* p - 1  */
    seq_t minus1 = p - (seq_t)1;

    /* floor division by n */
    seq_t div = minus1 / (seq_t)n;

    /* multiply back by n */
    seq_t res = div * (seq_t)n;

    return res;
}
