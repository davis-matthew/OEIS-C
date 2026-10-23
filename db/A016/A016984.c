/*
 * A016984  a(n) = (7*n)^4.
 * https://oeis.org/A016984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n)^4
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 36, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t1 = nn * (seq_t)7;    /* 7*n */
    seq_t t2 = t1 * t1;          /* (7*n)^2 */
    return t2 * t2;              /* (7*n)^4 */
}
