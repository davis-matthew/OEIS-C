/*
 * A033593  a(n) = (n-1)*(2*n-1)*(3*n-1)*(4*n-1).
 * https://oeis.org/A033593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)*(2*n-1)*(3*n-1)*(4*n-1) (offset 0)
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 116, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /*  For n = 0 the product equals 1, for n = 1 it equals 0. */
    if (n == 0) return (seq_t)1;
    /* For n >= 1 the expression works directly with unsigned arithmetic. */
    seq_t nn = (seq_t)n;
    seq_t f1 = nn - 1;           /* n-1   (0 when n=1) */
    seq_t f2 = 2 * nn - 1;       /* 2n-1  (1 when n=1) */
    seq_t f3 = 3 * nn - 1;       /* 3n-1  (2 when n=1) */
    seq_t f4 = 4 * nn - 1;       /* 4n-1  (3 when n=1) */
    return f1 * f2 * f3 * f4;
}
