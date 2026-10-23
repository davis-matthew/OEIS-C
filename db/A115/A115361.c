/*
 * A115361  Inverse of matrix (1,x)-(x,x^2) (expressed in Riordan array
 *          notation).
 * https://oeis.org/A115361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = 1 if the triangular array entry at the (row, column)
 *            position corresponding to the flattening index n is 1, and 0
 *            otherwise. The array is defined by T(r,c) = 1 if (c+1)|(r+1) and
 *            (r+1)/(c+1) is a power of two, 0 otherwise. Flattening is by
 *            rows: row 0 has 1 entry, row 1 has 2 entries, row...
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Find row r such that the cumulative number of entries before
       row r is <= n and the cumulative number including row r+1 is > n. */
    idx_t r = 0;          /* current row index */
    idx_t start = 0;      /* first index in the current row */

    while (start + (r + 1) <= n) {
        start += r + 1;   /* advance to the next row start */
        r++;
    }

    /* Column index within the row */
    idx_t c = n - start;

    /* Compute the entry value */
    idx_t row_plus1 = r + 1;
    idx_t col_plus1 = c + 1;

    if (row_plus1 % col_plus1 != 0)
        return (seq_t)0;                 /* not divisible → 0 */

    idx_t q = row_plus1 / col_plus1;      /* quotient */

    /* q is a power of two iff q & (q-1) == 0 */
    return (seq_t)((q & (q - 1)) == 0);
}
