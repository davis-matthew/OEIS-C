/*
 * A134231  Triangle T(n, k) = n -k +1 with T(n, n-1) = 2*n-1 and T(n, n) = 1,
 *          read by rows.
 * https://oeis.org/A134231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Let `r` be the unique integer such that `r*(r-1)/2 < n <=
 *            r*(r+1)/2`. Let `k = n - r*(r-1)/2`. Then a(n) = 1 if k == r
 *            a(n) = 2*r - 1 if k == r-1 a(n) = r - k + 1 otherwise
 * Range:     u8 n <= 1275, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t row = 1;        /* current row (starting at 1)           */
    idx_t cum = 1;        /* cumulative number of terms <= row      */

    /* Find the row that contains the n-th term.              */
    while (cum < n) {
        ++row;
        cum += row;
    }

    /* Number of terms before this row.                       */
    idx_t prev_cum = cum - row;

    /* Position of the term within the row (1‑based).          */
    idx_t k = n - prev_cum;

    if (k == row) {
        return (seq_t)1;
    }
    if (k == row - 1) {
        return (seq_t)(2 * row - 1);
    }
    return (seq_t)(row - k + 1);
}
