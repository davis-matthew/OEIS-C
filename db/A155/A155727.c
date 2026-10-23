/*
 * A155727  Production matrix of the Jacobsthal numbers, read by row.
 * https://oeis.org/A155727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabf
 * Formula:   The matrix has rows r ≥ 0 of length r+2. Row 0: [0, 1]. For r ≥
 *            1: [2r+1, (r−1) zeros, −1, 1]. a(n) is the element at position n
 *            in row‑major order
 * Range:     i8 n <= 53, i16 n <= 53, i32 n <= 53, i64 n <= 53, i128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* locate the row r containing n */
    idx_t row = 0;          /* current row index (0‑based)          */
    idx_t len  = 2;         /* length of current row (row + 2)     */
    idx_t start = 0;        /* start index of current row          */

    /* find the row r such that n < start + len */
    while (n >= start + len) {
        start += len;       /* advance to next row                  */
        ++row;              /* increment row index                  */
        len = row + 2;      /* new row length                       */
    }

    /* offset within the row    */
    idx_t k = n - start;

    /* return the value according to the row pattern */
    if (row == 0) {
        /* row 0: [0, 1] */
        return (k == 0) ? (seq_t)0 : (seq_t)1;
    } else {
        if (k == 0) {
            /* first element: 2r + 1 */
            seq_t r = (seq_t)row;
            return r * 2 + 1;
        } else if (k == row) {
            /* element just before the final 1: -1 */
            return (seq_t)-1;
        } else if (k == row + 1) {
            /* final element: 1 */
            return (seq_t)1;
        } else {
            /* all zeros in between */
            return (seq_t)0;
        }
    }
}
