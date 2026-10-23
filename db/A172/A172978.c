/*
 * A172978  a(n) = binomial(n+10, 10)*4^n.
 * https://oeis.org/A172978
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+10, 10)*4^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 19, u128 n <= 46,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    seq_t coeff = binomial(n + 10, 10);
    seq_t pow4  = pow_uint(4, n);
    return coeff * pow4;
}
