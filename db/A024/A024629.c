/*
 * A024629  n written in fractional base 3/2.
 * https://oeis.org/A024629
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 0, a(3*n + r) = 10*a(2*n) + r (r = 0,1,2)
 * Range:     u8 n <= 8, u16 n <= 23, u32 n <= 185, u64 n <= 7190,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t q = n / 3;
    int r = n % 3;
    return a(2 * q) * 10 + r;
}
