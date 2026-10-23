/*
 * A155151  Triangle T(n, k) = 4*n*k + 2*n + 2*k + 2, read by rows.
 * https://oeis.org/A155151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,c) = 4*r*c + 2*r + 2*c + 2 where r is the row number
 *            such that r(r+1)/2 ≥ n and c = n - r(r-1)/2 (column index in
 *            that row)
 * Range:     u8 n <= 34, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* offset starts at 1 */

    /* Find the row r for index n. */
    idx_t r = 0;          /* current row number */
    idx_t tri = 0;        /* cumulative number of elements up to row r */

    while (tri < n) {
        ++r;
        tri += r;         /* add elements of the next row */
    }

    /* Column number within the row. */
    idx_t prev = tri - r;     /* number of elements before row r */
    idx_t c = n - prev;       /* 1‑based column index */

    /* Compute the triangle entry. */
    seq_t val = 4 * (seq_t)r * (seq_t)c
                + 2 * (seq_t)r
                + 2 * (seq_t)c
                + 2;

    return val;
}
