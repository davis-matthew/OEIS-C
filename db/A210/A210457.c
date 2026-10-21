/*
 * A210457  Triangular array read by rows: T(n,k) is the number of elements x
 *          in {1,2,...,n} such that |(f^-1)(x)| = k over all functions
 *          f:{1,2,...,n}->{1,2,...,n}; n>=0, 0<=k<=n.
 * https://oeis.org/A210457
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binomial(r,k) * (r-1)^(r-k) * r, where r and k are the
 *            row and column indices obtained from n by reading the triangular
 *            array T(n,k) = binomial(n,k)*(n-1)^(n-k)*n by rows
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 54, u64 n <= 135, u128 n <= 377,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row r containing the index n.  The lengths of the rows are
       1, 2, 3, ... so the cumulative length before row r is r*(r+1)/2.
       As n is at most about 10 000, a simple linear search is fast enough. */
    idx_t cumulative = 0;
    idx_t r = 0;
    while (cumulative + r + 1 <= n) {
        cumulative += r + 1;
        ++r;
    }

    /* r is now the row number, and cumulative is the sum of lengths of
       all preceding rows.  The column index within this row is: */
    idx_t orig_col = n - cumulative;

    /* For r == 0 the formula yields 0 directly. */
    if (r == 0)
        return (seq_t)0;

    /* Compute binomial(r, orig_col) multiplicatively.  Use the smaller
       of k and r-k for efficiency. */
    idx_t bin_col = (orig_col > r - orig_col) ? r - orig_col : orig_col;
    seq_t binom = 1;
    for (idx_t i = 1; i <= bin_col; ++i) {
        binom = binom * (seq_t)(r - bin_col + i) / (seq_t)i;
    }

    /* Compute (r-1)^(r - orig_col).  The exponent r - orig_col is at most
       r <= ~141 for n up to 10 000, so a simple loop suffices. */
    seq_t power = 1;
    seq_t base = (seq_t)(r - 1);
    idx_t exp = r - orig_col;
    for (idx_t i = 0; i < exp; ++i)
        power *= base;

    /* Finally assemble the result. */
    seq_t result = binom * power * (seq_t)r;
    return result;
}
