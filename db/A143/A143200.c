/*
 * A143200  Triangle read by rows: T(n, k) = 1 if k = 0 or k = n, T(n, k) = -1
 *          if ( binomial(n, k) mod 2 ) = 1, otherwise T(n, k) = 0.
 * https://oeis.org/A143200
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, sign, less
 * Formula:   a(n) = T(row, col) where row, col satisfy n = row*(row+1)/2 +
 *            col; T(row, col) = 1 if col == 0 or col == row; T(row, col) = -1
 *            if (binomial(row, col) mod 2) == 1; otherwise T(row, col) = 0.
 *            The triangle is flattened row by row
 * Range:     i8 n <= 192, i16 n <= 1325, i32 n <= 1325, i64 n <= 1325,
 *            i128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row r containing index n */
    idx_t r = 0;
    while ((seq_t)(r + 1) * ((seq_t)(r + 1) + 1) / 2 <= (seq_t)n)
        ++r;

    /* Offset of the first element of row r */
    idx_t offset = (idx_t)((seq_t)r * ((seq_t)r + 1) / 2);
    idx_t k = n - offset;

    /* Edge entries are always 1 */
    if (k == 0 || k == r)
        return (seq_t)1;

    /* Interior entries: -1 if binomial(r, k) is odd, else 0 */
    if ((k & ~r) == 0)
        return (seq_t)(-1);
    else
        return (seq_t)0;
}
