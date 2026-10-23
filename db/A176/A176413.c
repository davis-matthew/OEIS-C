/*
 * A176413  a(n) = 19*3^n.
 * https://oeis.org/A176413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 19*3^n
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 17, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    seq_t v = pow3(n);
    v *= (seq_t)19;
    return v;
}
