/*
 * A168576  a(n) = (10^n+1)^4.
 * https://oeis.org/A168576
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (10^n + 1)^4
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 9,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t t = pow10(n);
    t = t + 1;
    seq_t t2 = t * t;
    seq_t t4 = t2 * t2;
    return t4;
}
