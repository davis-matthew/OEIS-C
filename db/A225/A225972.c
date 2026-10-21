/*
 * A225972  The number of binary pattern classes in the (2,n)-rectangular grid
 *          with 3 '1's and (2n-3) '0's: two patterns are in same class if one
 *          of them can be obtained by a reflection or 180-degree rotation of
 *          the other.
 * https://oeis.org/A225972
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)*(4*n^2 - 2*n - 3*(-1)^n + 3)/12
 * Range:     u8 n <= 9, u16 n <= 58, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use a 128‑bit intermediate to avoid overflow on large n. */
    __int128 N = ( __int128 )n;
    __int128 result;
    if ((n & 1ULL) == 0ULL)           /* n even  */
    {
        result = (N - 1) * N * (2 * N - 1) / 6;
    }
    else                              /* n odd   */
    {
        result = (N - 1) * (N * (2 * N - 1) + 3) / 6;
    }
    return (seq_t)result;
}
