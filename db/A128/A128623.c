/*
 * A128623  Triangle read by rows: A128621 * A000012 as infinite lower
 *          triangular matrices.
 * https://oeis.org/A128623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = r * (1 + floor((r - k)/2)), * where r is the row number
 *            (smallest r with r*(r+1)/2 ≥ n) * and k = n - r*(r-1)/2 is the
 *            1‑based column index in that row
 * Range:     u8 n <= 253, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                 /* offset starts at 1, but keep defined for safety */
    idx_t r = 1;
    idx_t tri = 1;                 /* triangular number r*(r+1)/2 */
    while (tri < n)
    {
        r++;
        tri = r * (r + 1) / 2;
    }
    /* tri now == r*(r+1)/2 >= n; previous row total = tri - r */
    idx_t k = n - (tri - r);       /* 1‑based column index within row r */
    idx_t diff = r - k;
    idx_t half = diff / 2;         /* floor division */
    seq_t val = (seq_t)r * ((seq_t)1 + (seq_t)half);
    return val;
}
