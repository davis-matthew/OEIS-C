/*
 * A176484  Triangle t(n,m) read by rows: t(n,n)= 1, t(n,m) =
 *          (-1)^(n+m)*(m+1), 0<=m<n.
 * https://oeis.org/A176484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   a(n) = t(k,m) where k = floor((sqrt(8*n+1)-1)/2), m = n -
 *            k*(k+1)/2, and t(k,m) = 1 if m==k else (-1)^(k+m)*(m+1)
 * Range:     i8 n <= 65, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t low = 0;
    idx_t high = n;
    idx_t row = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri <= n) {
            row = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    unsigned __int128 tri_row = (unsigned __int128)row * (row + 1) / 2;
    idx_t col = n - (idx_t)tri_row;
    if (col == row) {
        return (seq_t)1;
    }
    seq_t val = (seq_t)(col + 1);
    if (((row + col) & 1) == 0) {
        return val;
    } else {
        return -val;
    }
}
