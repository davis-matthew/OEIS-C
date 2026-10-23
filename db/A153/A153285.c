/*
 * A153285  a(1)=1; for n > 1, a(n) = n^2 + Sum_{j=1..n-1} (-1)^j*a(j).
 * https://oeis.org/A153285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; for n > 1, a(n) = 2n-1 if n is even, a(n) = 6n-7 if n
 *            is odd
 * Range:     u8 n <= 44, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* seq_t and idx_t are supplied by the build. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    seq_t nseq = (seq_t)n;

    if ((n & 1) == 0)          /* n is even */
        return 2 * nseq - (seq_t)1;

    /* n is odd and > 1 */
    return 6 * nseq - (seq_t)7;
}
