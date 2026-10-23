/*
 * A017344  a(n) = (10*n + 6)^4.
 * https://oeis.org/A017344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 6)^4
 * Range:     u8 none, u16 n <= 0, u32 n <= 24, u64 n <= 6552, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n * 10 + 6;   /* 10*n + 6  */
    seq_t x2 = x * x;              /* (10*n+6)^2 */
    return x2 * x2;                /* (10*n+6)^4 */
}
