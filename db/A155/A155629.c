/*
 * A155629  a(n) = 8^n - 4^n + 1.
 * https://oeis.org/A155629
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8^n - 4^n + 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute 8^n = 2^(3n) and 4^n = 2^(2n) using the supplied pow2 helper. */
    seq_t p8 = pow2(3 * n);
    seq_t p4 = pow2(2 * n);
    /* a(n) = 8^n - 4^n + 1 */
    return p8 - p4 + 1;
}
