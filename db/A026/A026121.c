/*
 * A026121  a(n) = 3^n*(3^n-1)/2.
 * https://oeis.org/A026121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n*(3^n-1)/2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    seq_t x = pow3(n);
    seq_t y = x * (x - (seq_t)1);
    y /= (seq_t)2;
    return y;
}
