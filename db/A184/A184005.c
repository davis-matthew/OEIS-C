/*
 * A184005  a(n) = n - 1 + ceiling(3*n^2/4); complement of A184004.
 * https://oeis.org/A184005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - 1 + ceil(3*n^2/4)
 * Range:     u8 n <= 9, u16 n <= 268, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* compute ceil(3*n^2/4) as (3*n*n + 3) / 4 */
    seq_t t = (seq_t)3 * nn * nn;
    seq_t ceil_val = (t + 3) / 4;
    return nn - 1 + ceil_val;
}
