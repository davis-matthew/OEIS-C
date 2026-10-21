/*
 * A222941  Number of nX3 0..4 arrays with no element equal to another at a
 *          city block distance of exactly two, and new values 0..4 introduced
 *          in row major order.
 * https://oeis.org/A222941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 3 a(2) = 33 a(3) = 420 for n > 3 : a(n) = 14 * a(n-1)
 *            equivalently a(n) = 420 * 14^(n-3)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)33;
    /* for n >= 3 */
    seq_t result = (seq_t)420;
    idx_t exp = n - 3;          /* n is unsigned, n>=3 ensures exp is correct */
    for (idx_t i = 0; i < exp; ++i)
        result *= (seq_t)14;    /* multiply by 14 exp times */
    return result;
}
