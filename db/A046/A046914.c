/*
 * A046914  Sum of aliquot factors (divisors excluding the number itself) of
 *          10^n.
 * https://oeis.org/A046914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2^(n+1)-1)*(5^(n+1)-1)/4 - 10^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    seq_t p2  = pow_uint((uint64_t)2, n + (idx_t)1);
    seq_t p5  = pow_uint((uint64_t)5, n + (idx_t)1);
    seq_t p10 = pow_uint((uint64_t)10, n);

    seq_t tmp = (p2 - (seq_t)1) * (p5 - (seq_t)1);
    tmp = tmp / (seq_t)4;
    tmp = tmp - p10;
    return tmp;
}
