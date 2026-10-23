/*
 * A115512  Number triangle (1,x)+(x,x^3) expressed in terms of Riordan
 *          arrays.
 * https://oeis.org/A115512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A115512: 1 for positions on the diagonal or on the line n = 3k+1
 *  in the triangle T(k,l) where T(k,l)=1 iff k==l or k==3l+1, otherwise 0.
 *  The sequence is the row‑major flattening of that triangle.
 */

#include <stdint.h>   /* for possible use of integer types */

/* Find the triangle row r that contains the flattened index n.
 * Each row r has length r+1, and the cumulative number of entries up to
 * row r is (r+1)(r+2)/2.  We find the smallest integer m such that
 * m(m+1)/2 > n and return r = m-1.  All arithmetic is done with
 * __int128 to avoid overflow for 64‑bit indices. */
static idx_t
find_row(idx_t n)
{
    idx_t low = 0;
    idx_t high = n + 1;          /* high is guaranteed to satisfy m(m+1)/2 > n */
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t > n)
            high = mid;
        else
            low = mid;
    }
    return high - 1;              /* r = m-1 */
}

/* Main sequence function */
seq_t
a(idx_t n)
{
    idx_t r = find_row(n);                      /* row index */
    idx_t prev_cum = r * (r + 1) / 2;           /* cumulative entries before row r */
    idx_t col = n - prev_cum;                   /* column index within the row */

    /* Return 1 if on the diagonal (col==r) or on the line n = 3*col+1,
     * otherwise 0. */
    return (col == r || r == 3 * col + 1) ? 1 : 0;
}
