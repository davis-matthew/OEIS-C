/*
 * A134374  a(n) = ((2n+1)!)^2.
 * https://oeis.org/A134374
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((2*n + 1)!)^2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    idx_t m = 2 * n + 1;          /* upper limit of factorial */
    seq_t result = 1;              /* factorial accumulator */

    for (idx_t i = 2; i <= m; ++i)
        result *= (seq_t)i;        /* compute (2*n+1)! */

    return result * result;        /* square the factorial */
}
