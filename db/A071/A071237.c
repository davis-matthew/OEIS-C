/*
 * A071237  a(n) = n*(n+1)*(n^2+1)/2.
 * https://oeis.org/A071237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n^2+1)/2
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 303, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the n-th term of OEIS sequence A071237.
 * The index n is of type idx_t (uint64_t).  The returned value is of
 * type seq_t, which may be uint8_t, uint16_t, uint32_t, uint64_t,
 * unsigned __int128, or an arbitrary-precision integer class.
 */
seq_t a(idx_t n)
{
    /* Convert the index to the seq_t type first. */
    seq_t nn = (seq_t)n;

    /* Compute n^2. */
    seq_t sq = nn * nn;

    /* Compute n*(n+1)*(n^2+1)/2.
     * The factor n*(n+1) is always even, so dividing by 2 preserves
     * exactness.  The intermediate products fit in virtually all
     * supported seq_t sizes for the expected input range. */
    seq_t prod = nn * (nn + 1) * (sq + 1);
    return prod / 2;
}
