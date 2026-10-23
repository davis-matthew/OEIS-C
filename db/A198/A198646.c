/*
 * A198646  a(n) = 11*3^n-1.
 * https://oeis.org/A198646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*3^n-1
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 18, u64 n <= 38, u128 n <= 78,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    seq_t val = pow3(n);
    val = val * (seq_t)11 - (seq_t)1;
    return val;
}
