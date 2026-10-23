/*
 * A028400  a(n) = (2^n + 1)^2.
 * https://oeis.org/A028400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^n + 1)^2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t p = pow2(n);   /* 2^n */
    seq_t v = p + 1;     /* 2^n + 1 */
    return v * v;        /* (2^n + 1)^2 */
}
