/*
 * A152390  Arises in enumerating non-degenerate colorings in Brook's Theorem.
 * https://oeis.org/A152390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n^3+8*n^2+19*n+6)*(n+1)
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;           /* n */
    seq_t n2 = nn * nn;            /* n^2 */
    seq_t n3 = n2 * nn;            /* n^3 */
    seq_t n4 = n3 * nn;            /* n^4 */

    /* a(n) = n^4 + 9n^3 + 27n^2 + 25n + 6 */
    return n4 + (9 * n3) + (27 * n2) + (25 * nn) + 6;
}
