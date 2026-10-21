/*
 * A075993  Triangle read by rows: T(n,m) is the number of integers k such
 *          that floor(n/k) = m, n >= 1, k = 1..n.
 * https://oeis.org/A075993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9499, u16 n <= 9499, u32 n <= 9499, u64 n <= 9499,
 *            u128 n <= 9499, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the nth term of the triangular array
   T(r,c) = floor(r/c) - floor(r/(c+1)),
   where the nth term in row‑major order lies in row r and column c.
   The row r is chosen such that (r-1)*r/2 < n <= r*(r+1)/2,
   and c = n - (r-1)*r/2.  */

seq_t a(idx_t n)
{
    /* Locate the row r containing the nth term. */
    idx_t r = 1;
    idx_t tri = 1;          /* triangular number r(r+1)/2 */
    while (tri < n) {
        r++;
        tri += r;           /* tri becomes r(r+1)/2 */
    }

    /* Compute the column c in that row. */
    idx_t prev_tri = tri - r;                      /* (r-1)*r/2 */
    idx_t offset = n - prev_tri - 1;               /* 0‑based offset in the row */
    idx_t c = offset + 1;                          /* column index, 1‑based */

    /* Evaluate T(r,c). */
    seq_t val = (seq_t)(r / c) - (seq_t)(r / (c + 1));
    return val;
}
