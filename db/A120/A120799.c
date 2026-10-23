/*
 * A120799  a(n) = 3^(n^2)-2^n.
 * https://oeis.org/A120799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^(n^2) - 2^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* exponent for 3 is n^2, compute safely in seq_t then cast to idx_t */
    idx_t exp3 = (idx_t)((seq_t)n * (seq_t)n);
    seq_t pow3 = pow_uint(3, exp3);
    seq_t pow2 = pow_uint(2, n);
    return pow3 - pow2;
}
