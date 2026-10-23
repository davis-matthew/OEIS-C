/*
 * A102094  a(n) = (2*n-1)*(2*n+1)^2.
 * https://oeis.org/A102094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2*n-1)*(2*n+1)^2
 * Range:     u8 n <= 3, u16 n <= 19, u32 n <= 812, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* cast index to the arithmetic type */
    seq_t two_n = nn + nn;        /* 2*n */
    seq_t t1 = two_n - 1;         /* 2*n-1 */
    seq_t t2 = two_n + 1;         /* 2*n+1 */
    return t1 * t2 * t2;          /* (2*n-1)*(2*n+1)^2 */
}
