/*
 * A185995  a(n) = 2^(binomial(2*n+2,3)/4).
 * https://oeis.org/A185995
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n*(2*n^2+3*n+1)/6)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Compute the exponent with a 128‑bit intermediate to avoid overflow. */
    __uint128_t nn  = n;
    __uint128_t exp = nn * ((__uint128_t)2 * nn * nn + (__uint128_t)3 * nn + 1);
    exp /= 6;
    return pow_uint(2, (idx_t)exp);
}
