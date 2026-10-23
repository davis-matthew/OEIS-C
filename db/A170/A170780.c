/*
 * A170780  a(n) = n^8*(n^8 + 1)/2.
 * https://oeis.org/A170780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^8*(n^8 + 1)/2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 16, u128 n <= 267,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute n^8 as seq_t */
    seq_t n8 = 1;
    for (idx_t i = 0; i < 8; ++i)
        n8 *= (seq_t)n;

    /* Compute n8*(n8+1)/2 avoiding overflow by dividing first */
    seq_t res;
    if ((n8 & 1) == 0)            /* n8 is even */
        res = (n8 / 2) * (n8 + 1);
    else                           /* n8 is odd, so (n8+1) is even */
        res = n8 * ((n8 + 1) / 2);

    return res;
}
