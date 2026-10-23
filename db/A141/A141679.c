/*
 * A141679  Triangle of coefficients of the inverse of A058071.
 * https://oeis.org/A141679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   not stated
 * Range:     i8 n <= 5050, i16 n <= 5050, i32 n <= 5050, i64 n <= 5050,
 *            i128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A141679 – Triangle of coefficients of the inverse of A058071.
    The triangle has the following structure for row n (n ≥ 1):
        T(n,1) .. T(n,n-2), T(n,n-2) = -1, T(n,n-1) = -1, T(n,n) = 1,
        all earlier columns are 0.
    The flattened sequence a(1),a(2),… is obtained by reading the triangle
    row‑by‑row.  Hence for a given position k we find the row r such that
    r(r+1)/2 ≥ k and the column c = k - (r-1)r/2.  Then

        a(k) =  1   if c == r
                -1  if c == r-1 or c == r-2
                0   otherwise
*/

#include <stdint.h>
#include <stddef.h>

static inline idx_t row_of(idx_t pos) {
    /* find smallest r such that r*(r+1)/2 >= pos  (pos >= 1) */
    idx_t low = 0, high = 1;
    while (( (__int128)high * (high + 1) / 2) < pos) high <<= 1;
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if (( (__int128)mid * (mid + 1) / 2) >= pos) high = mid;
        else low = mid;
    }
    return high;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;   /* undefined for n=0, defined as 0 for safety */

    /* Locate the row and the column within that row. */
    idx_t row = row_of(n);
    idx_t prev_offset = ( (__int128)(row - 1) * row ) / 2;   /* elements before this row */
    idx_t col = n - prev_offset;                            /* 1‑based column index */

    /* Determine the value according to the pattern described above. */
    if (col == row)            return (seq_t)1;
    if (col == row - 1)        return (seq_t)-1;
    if (col == row - 2)        return (seq_t)-1;
    return (seq_t)0;
}
