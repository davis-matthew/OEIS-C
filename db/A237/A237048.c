/*
 * A237048  Irregular triangle read by rows: T(n,k), n >= 1, k >= 1, in which
 *          column k lists 1's interleaved with k-1 zeros, and the first
 *          element of column k is in row k(k+1)/2.
 * https://oeis.org/A237048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) = T(row, col), where (row, col) is the position of the nth
 *            element in the irregular triangle defined on the OEIS entry
 *            A237048. T(row, col) = 1 if row - col*(col+1)/2 is a
 *            non‑negative multiple of col, otherwise 0
 * Range:     u8 n <= 1600, u16 n <= 1600, u32 n <= 1600, u64 n <= 1600,
 *            u128 n <= 1600, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row that contains the nth element.  Each row r has
       len(r) = floor((sqrt(8r+1)-1)/2) elements.  We incrementally
       build the lengths using the fact that len increases by 1 exactly
       when r reaches the next triangular number.  No floating point
       calculations are required. */
    idx_t row = 0;
    idx_t len = 0;          /* number of columns in the current row */
    idx_t next_transition = 1;   /* triangular number where len will grow next */
    idx_t cum = 0;          /* cumulative number of elements processed */

    while (cum < n) {
        row++;
        if (row >= next_transition) {
            len++;
            /* next triangular number after the current len */
            next_transition = (len + 1) * (len + 2) / 2;
        }
        cum += len;
    }

    /* Column index (1‑based) within the found row */
    idx_t col = n - (cum - len);

    /* Determine whether this position contains 1 or 0. */
    idx_t start = col * (col + 1) / 2;      /* first row where column col appears */
    if (row < start)
        return (seq_t)0;
    return (seq_t)((row - start) % col == 0);
}
