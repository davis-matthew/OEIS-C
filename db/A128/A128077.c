/*
 * A128077  A128064 * A002260.
 * https://oeis.org/A128077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th element of the triangle that consists of the
 *            rows 1; 1,4; 1,2,9; 1,2,3,16; … i.e. row r contains the numbers
 *            1,2,…,r‑1,r². Thus a(n) = r² if n = 1+2+…+r a(n) = n –
 *            (1+2+…+(r‑1)) otherwise, where r is the smallest integer with
 *            r(r+1)/2 ≥ n
 * Range:     u8 n <= 36, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* find the row number r such that triangular(r) ≥ n */
    idx_t r   = 1;       /* current row */
    idx_t tri = 1;       /* triangular(r) = r(r+1)/2 */
    while (tri < n) {
        ++r;
        tri += r;
    }
    /* tri is now triangular(r) and prev_tri = triangular(r-1) */
    const idx_t prev_tri = tri - r;
    const idx_t pos      = n - prev_tri;  /* position within the row (1‑based) */

    if (pos == r)          /* last element of the row: r² */
        return (seq_t)r * (seq_t)r;

    return (seq_t)pos;     /* one of the numbers 1…r‑1 */
}
