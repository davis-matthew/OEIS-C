/*
 * A020553  7th Fibonacci polynomial evaluated at x=n!.
 * https://oeis.org/A020553
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n!)^6 + 5*(n!)^4 + 6*(n!)^2 + 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t fac = 1;
    for (idx_t i = 2; i <= n; ++i) {
        fac *= i;
    }
    seq_t x2 = fac * fac;
    seq_t x4 = x2 * x2;
    seq_t x6 = x4 * x2;
    return x6 + 5 * x4 + 6 * x2 + 1;
}
