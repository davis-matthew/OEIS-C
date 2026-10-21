/*
 * A051933  Triangle T(n,m) = Nim-sum (or XOR) of n and m, read by rows,
 *          0<=m<=n.
 * https://oeis.org/A051933
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, nonn, easy, nice, hear, look
 * Formula:   a(n) = row(n) ^ (n - row_start(row(n))) where row(n) is the
 *            largest r with r*(r+1)/2 <= n. row_start(r) = r*(r+1)/2 is the
 *            index of the first element in row r
 * Range:     u8 n <= 8255, u16 n <= 8255, u32 n <= 8255, u64 n <= 8255,
 *            u128 n <= 8255, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row index r and the cumulative count of terms before this row. */
    idx_t r = 0;
    idx_t cum = 0;
    while (cum + (r + 1) <= n) {
        cum += r + 1;
        r++;
    }
    /* Position within the row. */
    idx_t m = n - cum;
    /* XOR of the row index and column index. */
    return (seq_t)r ^ (seq_t)m;
}
