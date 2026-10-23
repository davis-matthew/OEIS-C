/*
 * A118414  a(n) = (2*n - 1) * (2^n - 1).
 * https://oeis.org/A118414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n - 1) * (2^n - 1)
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 26, u64 n <= 57, u128 n <= 120,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* out of bounds, harmless */

    seq_t coef = (seq_t)2 * (seq_t)n - (seq_t)1;
    seq_t powv = pow2(n) - (seq_t)1;
    return coef * powv;
}
