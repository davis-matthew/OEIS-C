/*
 * A061793  a(n) = 25*n*(n + 1)/2 + 3.
 * https://oeis.org/A061793
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 25*n*(n+1)/2 + 3
 * Range:     u8 n <= 4, u16 n <= 71, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert index to the sequence integer type first */
    seq_t nn = (seq_t)n;

    /* Compute the triangular number n*(n+1)/2 */
    seq_t tri = nn * (nn + 1) / 2;

    /* Apply the linear transformation: 25*tri + 3 */
    return 25 * tri + 3;
}
