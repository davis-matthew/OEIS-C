/*
 * A139488  Binomial transform of [1, 2, 3, 4, 0, 0, 0, ...].
 * https://oeis.org/A139488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^3 - 3*n^2 + 11*n + 6)/6
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* cast index to seq_t first */
    seq_t term1 = (seq_t)4 * nn * nn * nn;   /* 4*n^3 */
    seq_t term2 = (seq_t)3 * nn * nn;        /* 3*n^2 */
    seq_t term3 = (seq_t)11 * nn;            /* 11*n */
    /* compute (4*n^3 - 3*n^2 + 11*n + 6) / 6 */
    seq_t result = (term1 - term2 + term3 + 6) / 6;
    return result;
}
