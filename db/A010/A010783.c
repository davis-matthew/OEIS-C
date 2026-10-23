/*
 * A010783  Triangle of numbers floor(n/(n-k)).
 * https://oeis.org/A010783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth element of the flattened triangle * T(r,k) =
 *            floor(r/(r-k+1)), where rows are numbered r = 1,2,3,... * and
 *            columns k = 1,…,r. The terms are listed row‑by‑row
 * Range:     u8 n <= 9289, u16 n <= 9289, u32 n <= 9289, u64 n <= 9289,
 *            u128 n <= 9289, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row r such that the cumulative number of terms up to that
       row (T(r) = r(r+1)/2) is at least n.  A simple O(sqrt(n)) loop is
       sufficient for n up to (and far beyond) 10 000. */
    idx_t r   = 1;
    idx_t sum = 1;          /* sum of 1+2+…+r */
    while (sum < n) {
        ++r;
        sum += r;
    }

    /* sum now equals the triangular number T(r) = r(r+1)/2.
       n - (sum - r) gives the 1‑based column position inside row r. */
    idx_t prev = sum - r;          /* total terms before row r */
    idx_t k    = n - prev;         /* column, 1‑based */

    /* Compute floor( r / (r - k + 1) ) using arithmetic in seq_t. */
    seq_t numerator   = (seq_t)r;
    seq_t denominator = (seq_t)(r - (k - 1));  /* r - k + 1, > 0 */
    return numerator / denominator;
}
