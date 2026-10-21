/*
 * A086602  a(n) = A000217(A000217(n))-n^2.
 * https://oeis.org/A086602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(n^2+3*n-2)/8
 * Range:     u8 n <= 6, u16 n <= 26, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n = 0 or 1 the formula gives 0 automatically */
    if (n <= 1) return 0;

    /* Use 128‑bit intermediate to avoid overflow for the advertised
       limits (n up to about 10000). */
    __int128 nn   = (__int128)n;
    __int128 tmp  = nn * (nn - 1);
    tmp           = tmp * (nn * nn + 3 * nn - 2);
    tmp           /= 8;

    return (seq_t)tmp;
}
