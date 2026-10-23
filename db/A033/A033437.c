/*
 * A033437  Number of edges in 5-partite Turán graph of order n.
 * https://oeis.org/A033437
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(2*n*n/5)
 * Range:     u8 n <= 11, u16 n <= 403, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;    /* convert index to seq_t */
    seq_t val = nn * nn;    /* n^2 */
    val *= 2;               /* 2*n^2 */
    return val / 5;         /* floor division */
}
