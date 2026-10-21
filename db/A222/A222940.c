/*
 * A222940  Number of n X 2 0..4 arrays with no element equal to another at a
 *          city block distance of exactly two, and new values 0..4 introduced
 *          in row major order.
 * https://oeis.org/A222940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 + 10*3^(2*n-5) for n > 2; a(1)=2, a(2)=7
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   pow_seq3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq3.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)7;
    /* exponent = 2*n - 5 */
    idx_t exp = n + n;
    exp -= 5;
    seq_t val = pow_seq3(exp);
    val *= (seq_t)10;
    val += (seq_t)3;
    return val;
}
