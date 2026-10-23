/*
 * A155726  Production matrix for Fibonacci numbers, read by row.
 * https://oeis.org/A155726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabf
 * Formula:   a(n) is the entry obtained by reading the infinite matrix M row
 *            by row, where: - M[0] = [0, 1] - For r ≥ 1: M[r][0] = r + 1
 *            M[r][c] = 0 for 1 ≤ c ≤ r - 1 M[r][r] = -1 M[r][r + 1] = 1 The
 *            matrix has row length r + 2
 * Range:     i8 n <= 52, i16 n <= 52, i32 n <= 52, i64 n <= 52, i128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) by determining which row and column
   n lies in and returning the corresponding entry. */
seq_t a(idx_t n)
{
    /* Find the row r such that cumulative entries up to
       row (r-1) are ≤ n < cumulative entries up to row r. */
    idx_t r = 0;
    idx_t cumulative = 0;          /* cumulative count up to row r-1 */

    while (1) {
        idx_t row_len = r + 2;     /* length of row r */
        idx_t next_cumulative = cumulative + row_len;

        if (n < next_cumulative)
            break;

        cumulative = next_cumulative;
        ++r;
    }

    /* Index within the found row */
    idx_t col = n - cumulative;

    /* Return the appropriate matrix entry */
    if (r == 0) {
        /* Row 0 has only two entries: 0, 1 */
        return (col == 0) ? (seq_t)0 : (seq_t)1;
    } else {
        if (col == 0)
            return (seq_t)(r + 1);          /* first column */
        else if (col <= r - 1)
            return (seq_t)0;                /* zero body */
        else if (col == r)
            return (seq_t)(-1);             /* sub‑diagonal */
        else                                   /* col == r + 1 */
            return (seq_t)1;                /* last column */
    }
}
