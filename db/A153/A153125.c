/*
 * A153125  Triangle read by rows: T(n,k) = maximal number of squares that can
 *          be covered by a queen on an n X k chessboard, 1<=k<=n.
 * https://oeis.org/A153125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r, k) where r = smallest integer with r*(r+1)/2 >= n
 *            (row number) k = n - r*(r-1)/2 (column number in that row)
 *            T(r,k) = r + 3*(k-1) - [(r is even) and (k==r) ? 1 : 0] The
 *            triangle row‑wise produces the sequence
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find row r using binary search.  All arithmetic that can overflow
     * 64 bits is performed in unsigned __int128.                      */
    idx_t lo = 1, hi = n;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 tri_mid = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri_mid >= n)  hi = mid;
        else               lo = mid + 1;
    }
    idx_t r = lo;                          /* row */

    /* Previous triangular number r-1 choose 2  (= (r-1)*r/2)                */
    unsigned __int128 tri_prev = (unsigned __int128)(r - 1) * r / 2;
    idx_t k = n - (idx_t)tri_prev;          /* column */

    seq_t value = (seq_t)r + 3 * ((seq_t)(k - 1));
    if ((r & 1) == 0 && k == r)            /* subtract delta_{r,k} if r even   */
        value -= 1;
    return value;
}
