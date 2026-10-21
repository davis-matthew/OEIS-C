/*
 * A095873  Triangle T(n,k) = (2*k-1)*(n+k-1)*(n-k+1) read by rows, 1<=k<=n.
 * https://oeis.org/A095873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = (2k-1)*(n+k-1)*(n-k+1) where k = n - (r-1)*r/2 and r is
 *            the smallest integer with r(r+1)/2 >= n
 * Range:     u8 n <= 24, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset is 1, but guard against 0 */
    idx_t r = 1;                          /* current row number   */
    idx_t sum = 1;                        /* triangular sum of rows up to r */

    /* Find the row r such that sum of 1..r >= n */
    while (sum < n) {
        ++r;
        sum += r;
    }

    idx_t prev_sum = sum - r;             /* sum of rows 1..r-1 */
    idx_t c = n - prev_sum;               /* column index in row r */

    /* Convert to seq_t for safe multiplication */
    seq_t sr = (seq_t) r;
    seq_t sc = (seq_t) c;

    /* Compute the triangle entry (2c-1)*(r+c-1)*(r-c+1) */
    seq_t term = (2*sc - 1) * (sr + sc - 1) * (sr - sc + 1);
    return term;
}
