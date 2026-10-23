/*
 * A017612  a(n) = (12*n + 7)^8.
 * https://oeis.org/A017612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 7)^8
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 20, u128 n <= 5460,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute x = 12*n + 7  in seq_t space */
    seq_t x = (seq_t)12 * (seq_t)n + (seq_t)7;

    /* compute x^8 via repeated squaring */
    seq_t x2 = x * x;   /* x^2  */
    seq_t x4 = x2 * x2; /* x^4  */
    seq_t x8 = x4 * x4; /* x^8  */

    return x8;
}
