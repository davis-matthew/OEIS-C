/*
 * A162436  a(n) = 3*a(n-2) for n > 2; a(1) = 1, a(2) = 3.
 * https://oeis.org/A162436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-2) for n>2; a(1)=1, a(2)=3 Equivalently: a(n) =
 *            3^floor(n/2)
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 41, u64 n <= 81, u128 n <= 161,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* floor division by 2 gives the exponent of 3 */
    return pow3(n / 2);
}
