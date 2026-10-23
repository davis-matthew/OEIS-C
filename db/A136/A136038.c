/*
 * A136038  a(n) = n^6 - n^4.
 * https://oeis.org/A136038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6 - n^4
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 40, u64 n <= 1625,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* convert index to seq_t */
    seq_t n2 = nn * nn;           /* n^2 */
    seq_t n4 = n2 * n2;           /* n^4 */
    seq_t n6 = n4 * n2;           /* n^6 */
    return n6 - n4;               /* n^6 - n^4 */
}
