/*
 * A188377  a(n) = n^3 - 4*n^2 + 6*n - 2.
 * https://oeis.org/A188377
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 - 4*n^2 + 6*n - 2
 * Range:     u8 n <= 7, u16 n <= 41, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;     /* n      */
    seq_t n2 = nn * nn;      /* n^2    */
    seq_t n3 = n2 * nn;      /* n^3    */
    seq_t res = n3 - (seq_t)4 * n2 + (seq_t)6 * nn - (seq_t)2;
    return res;
}
