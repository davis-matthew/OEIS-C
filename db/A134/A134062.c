/*
 * A134062  Row sums of triangle A134061.
 * https://oeis.org/A134062
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 * a(n) = 5*2^n - 2 for n > 0
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0) {
        return (seq_t)1;
    }
    seq_t p = pow2(n);          /* 2^n */
    return (seq_t)5 * p - (seq_t)2;
}
