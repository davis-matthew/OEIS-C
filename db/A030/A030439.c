/*
 * A030439  a(n+1) = smallest number not containing any digits of a(n),
 *          working in base 3.
 * https://oeis.org/A030439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 3^floor(n/2) - 1 + (n mod 2)
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    seq_t result = pow3(n / 2);
    result -= 1;
    if (n & 1) result += 1;
    return result;
}
