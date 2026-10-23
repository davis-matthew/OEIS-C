/*
 * A133728  A128174 * A127775.
 * https://oeis.org/A133728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (row%2 == col%2) ? 2*col-1 : 0 ; (row, col) are the
 *            1‑based row and column indices of n in the lower‑triangular
 *            array A127775: A127775[i][j] = (i%2 == j%2) ? 2*j-1 : 0
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /*  For safety, treat n == 0 (outside offset) as 0 */
    if (n == 0) return (seq_t)0;

    /* Find the row r such that T(r-1) < n ≤ T(r), where
       T(r) = r*(r+1)/2 is the r‑th triangular number. */
    idx_t lo = 1, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi) / 2;
        idx_t tri = mid * (mid + 1) / 2;
        if (tri >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t r = lo;                     /* row (1‑based)   */
    idx_t prev_tri = (r <= 1) ? 0 : (r - 1) * r / 2;
    idx_t c = n - prev_tri;           /* column (1‑based) */

    if ((r & 1) == (c & 1))
        return ((seq_t)2 * c) - 1;    /* non‑zero value */
    else
        return (seq_t)0;              /* zero value     */
}
