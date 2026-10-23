/*
 * A016809  a(n) = (4n)^9.
 * https://oeis.org/A016809
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n)^9
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* (4*n)^9 = 4^9 * n^9 = 262144 * n^9 */
    seq_t t = (seq_t)n;                 /* n */
    t *= t;                             /* n^2 */
    seq_t t4 = t * t;                   /* n^4 */
    seq_t t8 = t4 * t4;                 /* n^8 */
    seq_t n9 = t8 * (seq_t)n;           /* n^9 */
    return n9 * (seq_t)262144;          /* 4^9 * n^9 */
}
