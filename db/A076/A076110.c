/*
 * A076110  Triangle (read by rows) in which the n-th row contains first n
 *          terms of an arithmetic progression with first term 1 and common
 *          difference (n-1).
 * https://oeis.org/A076110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the term obtained by reading the triangular array T(r,c)
 *            = 1 + (r-1)*(c-1) (r,c ≥ 1) row‑by‑row. For a given linear index
 *            n ≥ 1 find the row r where S(r-1) < n ≤ S(r), with S(r) =
 *            r*(r+1)/2, then c = n - S(r-1), and return T(r,c)
 * Range:     u8 n <= 151, u16 n <= 9207, u32 n <= 9207, u64 n <= 9207,
 *            u128 n <= 9207, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for offset 1, but harmless */

    /* Find a row r such that S(r) >= n, using binary search. */
    idx_t low = 1, high = 1;
    while ((seq_t)high * (high + 1) / 2 < n) high <<= 1;

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        seq_t s = (seq_t)mid * ((seq_t)mid + 1) / 2;
        if (s >= n) high = mid;
        else low = mid + 1;
    }
    idx_t row = low;

    /* Compute column */
    idx_t prev = (idx_t)(((seq_t)(row - 1) * row) / 2);
    idx_t col  = n - prev;

    /* Compute the term T(row, col) */
    return (seq_t)1 + (seq_t)(row - 1) * (col - 1);
}
