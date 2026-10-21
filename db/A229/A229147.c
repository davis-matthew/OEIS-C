/*
 * A229147  a(n) = n^4*(3*n+2).
 * https://oeis.org/A229147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4*(3*n+2)
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 67, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n;         /* n as seq_t */
    seq_t n2   = nn * nn;          /* n^2 */
    seq_t n4   = n2 * n2;          /* n^4 */
    seq_t term = ( (seq_t)3 * nn ) + (seq_t)2;  /* 3*n + 2 */
    return n4 * term;              /* n^4 * (3*n+2) */
}
