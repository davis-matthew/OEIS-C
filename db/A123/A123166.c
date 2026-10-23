/*
 * A123166  Row sums of A123162.
 * https://oeis.org/A123166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(n) = 4^(n-1)+1 for n >= 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    idx_t exp = 2 * (n - (idx_t)1);   /* exponent 2n-2 */
    seq_t val = pow2(exp);
    val += (seq_t)1;
    return val;
}
