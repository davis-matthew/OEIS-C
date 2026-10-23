/*
 * A117908  Chequered (or checkered) triangle for odd prime p=3.
 * https://oeis.org/A117908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = 1 if (row mod 3 < 2 and col mod 3 < 2), else 0
 * Range:     u8 n <= 1325, u16 n <= 1325, u32 n <= 1325, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is obtained by reading the chequered triangle for p=3 row by row.
   Row R contains R+1 entries; col is the position within that row.      */

#include <stdint.h>

/* Compute floor(sqrt(x)) for an unsigned __int128 value x.  */
static inline idx_t
floor_sqrt_u128(unsigned __int128 x)
{
    unsigned __int128 lo = 0, hi = ((unsigned __int128)1 << 64) - 1;
    while (lo < hi) {
        unsigned __int128 mid = (lo + hi + 1) >> 1;
        if (mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return (idx_t)lo;
}

/* Main sequence function. */
seq_t
a(idx_t n)
{
    /*  Find the row R such that R*(R+1)/2 <= n < (R+1)*(R+2)/2. */
    unsigned __int128 t = (unsigned __int128)8 * n + 1;           /* 8*n+1 */
    idx_t r = floor_sqrt_u128(t);                               /* floor sqrt */
    idx_t R = (r - 1) / 2;                                       /* row number */

    /*  Compute the offset of the start of row R.                     */
    unsigned __int128 off128 = (unsigned __int128)R * (R + 1) / 2;
    idx_t offset = (idx_t)off128;

    /*  Column within the row. */
    idx_t col = n - offset;

    /*  Apply the chequered condition. */
    if ((R % 3) < 2 && (col % 3) < 2)
        return (seq_t)1;
    else
        return (seq_t)0;
}
