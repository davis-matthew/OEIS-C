/*
 * A108669  Triangle read by rows: T(n,k) = 11*k*n + 14*(n+k) + 20 (0 <= k <=
 *          n).
 * https://oeis.org/A108669
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) reads the triangle T(n,k) = 11*k*n + 14*(n+k) + 20 by rows.
 *            The mapping from a linear index n to triangle coordinates (row
 *            r, col k) is performed by computing the largest r such that
 *            r*(r+1)/2 ≤ n. Then k = n - r*(r+1)/2. The value is computed
 *            directly in seq_t
 * Range:     u8 n <= 13, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t row_of_n(idx_t n)
{
    idx_t r = 0;
    /* Find the largest r with r*(r+1)/2 ≤ n. */
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }
    return r;
}
 
seq_t a(idx_t n)
{
    idx_t r = row_of_n(n);
    idx_t prev = r * (r + 1) / 2;      /* number of terms before row r */
    idx_t k   = n - prev;               /* column within the row   */
 
    seq_t sr = (seq_t)r;
    seq_t sk = (seq_t)k;
 
    /* T(r,k) = 11*k*r + 14*(r + k) + 20 */
    return 11 * sk * sr + 14 * (sr + sk) + 20;
}
