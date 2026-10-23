/*
 * A025192  a(0)=1; a(n) = 2*3^(n-1) for n >= 1.
 * https://oeis.org/A025192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy, eigen
 * Formula:   a(0) = 1; a(n) = 2*3^(n-1) for n >= 1
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 81,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    seq_t val = pow3(n - 1);
    val *= (seq_t)2;
    return val;
}
