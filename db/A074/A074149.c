/*
 * A074149  Sum of terms in each group in A074147.
 * https://oeis.org/A074149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 + n)/2 if n is odd * a(n) = n^3/2 + n if n is even *
 *            This implements the explicit formula from the OEIS entry A074149
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert index to the sequence type before any arithmetic */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;

    if (n & 1ULL)        /* odd n */
    {
        return (n3 + nn) / 2;
    }
    else                 /* even n */
    {
        return (n3 / 2) + nn;
    }
}
