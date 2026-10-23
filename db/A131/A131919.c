/*
 * A131919  A002024 + A131821 - A000012.
 * https://oeis.org/A131919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2*r+1 if n is at an edge of the row r (c=0 or c=r),
 *            otherwise a(n)=r+1, where the row index r satisfies r*(r+1)/2 ≤
 *            n < (r+1)*(r+2)/2
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <inttypes.h>

static inline idx_t row_of(idx_t n)
{
    /* Find the largest r with r*(r+1)/2 ≤ n.  Loop is O(√n); n is small. */
    idx_t r = 0;
    while ( (__uint128_t)r * (r + 1) / 2 <= n )
        r++;
    return r - 1;
}

seq_t a(idx_t n)
{
    idx_t r = row_of(n);
    idx_t start = r * (r + 1) / 2;   /* index of first element of row r */
    idx_t c   = n - start;          /* column within the row */

    if (c == 0 || c == r)
        return (seq_t)(2 * (seq_t)r + 1);
    else
        return (seq_t)(r + 1);
}
