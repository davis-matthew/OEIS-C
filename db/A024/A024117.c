/*
 * A024117  a(n) = 10^n - n^3.
 * https://oeis.org/A024117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^n - n^3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n3  = nn * nn * nn;
    seq_t p10 = pow10(n);
    return p10 - n3;
}
