/*
 * A017022  a(n) = (7*n + 3)^6.
 * https://oeis.org/A017022
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n + 3)^6
 * Range:     u8 none, u16 n <= 0, u32 n <= 5, u64 n <= 231, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t x  = (seq_t)7 * n + 3;   /* base = 7*n + 3 */
    seq_t x2 = x * x;              /* (7n+3)^2 */
    seq_t x4 = x2 * x2;            /* (7n+3)^4 */
    seq_t x6 = x4 * x2;            /* (7n+3)^6 */
    return x6;
}
