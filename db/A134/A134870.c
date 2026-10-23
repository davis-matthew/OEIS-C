/*
 * A134870  A051731 + A000012 - A103451.
 * https://oeis.org/A134870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 + [r % c == 0 && 1 < c < r], where r = smallest integer
 *            with r*(r+1)/2 >= n and c = n - (r-1)*r/2. r is the row number
 *            of the lower‑triangular indexing of the triangle, and c is the
 *            column index within that row
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the row r such that the triangular number T(r)=r*(r+1)/2
       is the first one ≥ n.  For n up to 10000 this loop runs < 200
       iterations. */
    idx_t r = 1;
    while ( ((idx_t)r * (r + 1)) >> 1 < n )
        ++r;

    /* Compute the linear index where this row starts. */
    idx_t start_of_row = ((idx_t)(r - 1) * r) >> 1;   /* T(r-1) */
    idx_t c = n - start_of_row;   /* column index starting from 1 */

    /* Determine if (r,c) corresponds to a proper divisor of r. */
    if (c > 1 && c < r && r % c == 0)
        return (seq_t)2;
    else
        return (seq_t)1;
}
