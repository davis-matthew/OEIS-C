/*
 * A014551  Jacobsthal-Lucas numbers.
 * https://oeis.org/A014551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 2^n + (-1)^n
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t v = pow2(n);
    if (n & 1) {
        /* n odd: subtract 1 */
        v -= (seq_t)1;
    } else {
        /* n even: add 1 */
        v += (seq_t)1;
    }
    return v;
}
