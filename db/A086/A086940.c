/*
 * A086940  a(n) = k where R(k+4) = 2.
 * https://oeis.org/A086940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 2*(10^n - 2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* 10^n as a seq_t */
    seq_t ten = pow10(n);
    /* a(n) = 2*10^n - 4 */
    return ten * 2 - 4;
}
