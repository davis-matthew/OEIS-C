/*
 * A117860  Number of palindromes of length n (in base 8).
 * https://oeis.org/A117860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 7*8^floor((n-1)/2)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* n is at least 1 by OEIS definition.  Compute floor((n-1)/2). */
    idx_t e = (n - 1) / 2;           /* exponent of 8 */
    seq_t pow8 = pow_uint(8, e);     /* 8^e */
    return (seq_t)7 * pow8;          /* 7 * 8^e */
}
