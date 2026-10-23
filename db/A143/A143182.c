/*
 * A143182  Triangle T(n,m) = 1 + abs(n-2*m), read by rows, 0<=m<=n.
 * https://oeis.org/A143182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = 1 + | r - 2*m | where (r,m) is the position of the nth
 *            entry of the triangle T(r,m) = 1 + | r - 2*m | read row‑wise
 *            (offset 0). The row index r is the smallest integer satisfying
 *            r(r+1)/2 > n, and m = n - r(r+1)/2
 * Range:     u8 n <= 5150, u16 n <= 5150, u32 n <= 5150, u64 n <= 5150,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = 0;            /* row index          */
    idx_t cum = 0;          /* cumulative count before current row */

    /* find r by incremental search : O(sqrt(n)) time */
    while (cum + r + 1 <= n) {
        cum += r + 1;
        r++;
    }

    idx_t m = n - cum;      /* column index within row r */

    /* compute absolute difference */
    seq_t diff;
    if (r > 2 * m)
        diff = (seq_t)(r - 2 * m);
    else
        diff = (seq_t)(2 * m - r);

    return 1 + diff;
}
