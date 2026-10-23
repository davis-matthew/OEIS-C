/*
 * A120949  a(n) = 2n+3^n+5^n.
 * https://oeis.org/A120949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n + 3^n + 5^n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    seq_t pow3   = pow_int((seq_t)3, n);
    seq_t pow5   = pow_int((seq_t)5, n);
    seq_t linear = (seq_t)2 * (seq_t)n;
    return linear + pow3 + pow5;
}
