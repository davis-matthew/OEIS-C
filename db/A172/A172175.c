/*
 * A172175  a(n) = 110*a(n-1) + 1.
 * https://oeis.org/A172175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 110*a(n-1)+1; a(1)=1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* For the OEIS offset of 1, the sequence starts at a(1)=1. */
    seq_t res = (seq_t)1;
    /* Compute successive terms up to n. */
    for (idx_t i = 2; i <= n; ++i)
    {
        res = res * (seq_t)110 + (seq_t)1;
    }
    return res;
}
