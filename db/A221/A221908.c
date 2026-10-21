/*
 * A221908  a(n) = 6^n + 6*n.
 * https://oeis.org/A221908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^n + 6*n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    seq_t p6 = pow_int((seq_t)6, n);   /* 6^n */
    return p6 + (seq_t)6 * (seq_t)n;   /* + 6*n */
}
