/*
 * A103516  Triangle read by rows: count in a vee.
 * https://oeis.org/A103516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = (row+1) if n is at the left or right edge of the *
 *            triangular array T(row,col) = (col==0||col==row)?row+1:0, *
 *            otherwise a(n)=0. The row is the unique r with r(r+1)/2 ≤ n <
 *            (r+1)(r+2)/2
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t low = 0, high = n;               /* r ≤ n always holds */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1) + ((high - low) & 1); /* ceil midpoint */
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;
    idx_t row_start = (r * (r + 1)) / 2;
    idx_t col = n - row_start;
    if (col == 0 || col == r)
        return (seq_t)(r + 1);
    else
        return 0;
}
