/*
 * A154957  A symmetric (0,1)-triangle.
 * https://oeis.org/A154957
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r, c) where * r = floor((sqrt(8*n + 1) - 1)/2) (row
 *            index) * c = n - r*(r+1)/2 (column index within the row) * T(r,
 *            c) = 1 if min(c, r−c) is even, else 0
 * Range:     u8 n <= 1325, u16 n <= 1325, u32 n <= 1325, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function finds the row r by binary searching the triangular numbers.
 * The triangle entry is then computed using only integer operations,
 * thus avoiding any floating point or external dependencies. */

seq_t a(idx_t n)
{
    /* find largest r with r*(r+1)/2 <= n */
    idx_t r = 0, low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri <= n) {
            r = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }

    /* offset of first element of row r */
    unsigned __int128 off128 = (unsigned __int128)r * (r + 1) / 2;
    idx_t offset = (idx_t)off128;

    /* column index within the row */
    idx_t k = n - offset;

    /* find the smaller of k and r-k */
    idx_t m = (k < (r - k)) ? k : (r - k);

    /* T(r, k) = 1 if m is even, else 0 */
    return (m & 1) ? (seq_t)0 : (seq_t)1;
}
