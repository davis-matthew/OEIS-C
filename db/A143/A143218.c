/*
 * A143218  Triangle read by rows, A127775 * A000012 * A127775; 1<=k<=n.
 * https://oeis.org/A143218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (2*row-1)*(2*col-1), where (row,col) is obtained by
 *            mapping the 1‑based index n to a triangular array: find the
 *            minimal row such that row*(row+1)/2 >= n, then col = n -
 *            (row-1)*row/2
 * Range:     u8 n <= 44, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* The following function is provided by the build. */
/* #include "../utils/..."  // not needed for this implementation */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0; /* value for indices below offset, unused */
    }
    idx_t i = n;             /* 1‑based linear index */
    /* Binary search to find the minimal row r with r*(r+1)/2 >= i */
    idx_t low  = 1;
    idx_t high = i;           /* upper bound; safe for idx_t values */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        /* Use 128‑bit arithmetic to avoid overflow */
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= (unsigned __int128)i) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    idx_t row = low;
    /* Compute previous triangular number (row-1)*row/2 */
    unsigned __int128 prev_tri128 = (unsigned __int128)(row - 1) * row / 2;
    idx_t prev_tri = (idx_t)prev_tri128;
    idx_t col = i - prev_tri;          /* 1‑based column index */

    seq_t two = (seq_t)2;
    seq_t res = (two * (seq_t)row - (seq_t)1) *
                (two * (seq_t)col - (seq_t)1);
    return res;
}
