/*
 * A053764  a(n) = 3^(n^2 - n).
 * https://oeis.org/A053764
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3^(n^2 - n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* the exponent is n*(n-1); unsigned arithmetic handles n==0 safely */
    idx_t exp = n * (n - 1);
    return pow_uint(3, exp);
}
