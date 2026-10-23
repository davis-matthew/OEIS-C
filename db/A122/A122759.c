/*
 * A122759  Triangle T(n,m) read by rows: 3^n if m is odd, 0 if m is even.
 * https://oeis.org/A122759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy, less
 * Formula:   a(n) = 0 if the row index r is even, else a(n) = 3^(pos-1) where
 *            r is the smallest integer with r(r+1)/2 ≥ n and pos = n -
 *            r(r-1)/2. (Rows are 1‑indexed.)
 * Range:     u8 n <= 27, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row r such that the cumulative number of terms up to
       row r is at least n.  cum = r(r+1)/2 grows monotonically. */
    idx_t r = 1;
    idx_t cum = 1;          /* cumulative terms up to current r */
    while (cum < n) {
        ++r;
        cum += r;           /* now cum = r(r+1)/2 */
    }

    /* Number of terms before this row */
    idx_t prev_cum = cum - r;

    /* Position within the row (1‑based) */
    idx_t pos = n - prev_cum;

    /* Even rows contain only zeros */
    if ((r & 1) == 0)
        return (seq_t)0;

    /* Odd rows: compute 3^(pos-1) */
    seq_t val = 1;
    for (idx_t i = 0; i < pos - 1; ++i)
        val *= (seq_t)3;
    return val;
}
