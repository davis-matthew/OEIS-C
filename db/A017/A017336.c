/*
 * A017336  a(n) = (10*n + 5)^8.
 * https://oeis.org/A017336
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 5)^8
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 25, u128 n <= 6553,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t v = (seq_t)10 * (seq_t)n + (seq_t)5;   /* (10*n + 5) */
    seq_t v2 = v * v;                           /* (10*n + 5)^2 */
    seq_t v4 = v2 * v2;                         /* (10*n + 5)^4 */
    seq_t v8 = v4 * v4;                         /* (10*n + 5)^8 */
    return v8;
}
