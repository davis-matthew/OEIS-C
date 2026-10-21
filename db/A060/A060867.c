/*
 * A060867  a(n) = (2^n - 1)^2.
 * https://oeis.org/A060867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^n - 1)^2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 0)           /* undefined for the sequence, but safe return */
        return (seq_t)0;
    seq_t p = pow_uint((uint64_t)2, n); /* p = 2^n */
    seq_t d = p - (seq_t)1;           /* d = 2^n - 1 */
    return d * d;                     /* a(n) = (2^n - 1)^2 */
}
