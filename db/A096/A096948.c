/*
 * A096948  Triangular table read by rows: T(n,m) = number of rectangles found
 *          in an n X m rectangle built from 1 X 1 squares, 1 <= m <= n.
 * https://oeis.org/A096948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = t(r)*t(m) where t(x)=x*(x+1)/2, r = smallest integer with
 *            r(r+1)/2 ≥ n (row number), m = n - (r-1)*r/2 (column index
 *            within the row)
 * Range:     u8 n <= 19, u16 n <= 274, u32 n <= 9636, u64 n <= 9636,
 *            u128 n <= 9636, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Find row r such that r(r+1)/2 >= n (smallest such r). */
    idx_t lo = 1, hi = n;          /* for n>=1, r <= n                    */
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        /* mid*(mid+1)/2 may overflow idx_t for very large mid,
           but n is assumed to fit comfortably in idx_t.          */
        seq_t tri = ((seq_t)mid * (mid + 1)) / 2;
        if (tri >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t r = lo;

    /* Column index m in this row                    */
    idx_t m = n - ((r - 1) * r) / 2;

    /* Compute triangular numbers using seq_t to avoid overflow. */
    seq_t tr = ((seq_t)r * (r + 1)) / 2;
    seq_t tm = ((seq_t)m * (m + 1)) / 2;
    return tr * tm;
}
