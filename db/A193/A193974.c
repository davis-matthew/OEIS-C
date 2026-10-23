/*
 * A193974  Mirror of the triangle A193973.
 * https://oeis.org/A193974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = w(r, r-k) with * r = floor((sqrt(8*n+1)-1)/2) (row index)
 *            * k = n - r*(r+1)/2 (position in the row) * w(r,t) =
 *            ((t+1)*(2*r+4-t))/2 * where 0 ≤ t ≤ r. This follows from the
 *            definition of * triangle A193973 and the mirroring in A193974
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while ((seq_t)r * (r + 1) / 2 <= n)
        r++;
    r--;                           /* now r satisfies the inequality */

    idx_t offset = r * (r + 1) / 2;/* start index of row r */
    idx_t k      = n - offset;     /* position within the row, 0‑based */
    idx_t t      = r - k;          /* mirrored position */

    seq_t rr = (seq_t)r;
    seq_t tt = (seq_t)t;

    /* w(r,t) = ((t+1)*(2*r+4-t)) / 2  */
    seq_t numerator = (tt + 1) * (2 * rr + 4 - tt);
    return numerator / 2;
}
