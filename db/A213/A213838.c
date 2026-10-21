/*
 * A213838  Rectangular array: (row n) = b**c, where b(h) = 4*h-3, c(h) =
 *          2*n-3+2*h, n>=1, h>=1, and ** = convolution.
 * https://oeis.org/A213838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = (2r + 2c – 1) * c * (2c – 1) – c * (c + 1) * (8c – 5) / 3
 *            where (r, c) are the row and column indices of the element
 *            reached by reading the array of convolutions in antidiagonals
 *            (top‑to‑bottom within each antidiagonal). The array entry at row
 *            r, column c is obtained by the above closed‑fo...
 * Range:     u8 n <= 15, u16 n <= 253, u32 n <= 253, u64 n <= 253,
 *            u128 n <= 253, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the antidiagonal index t such that
       (t+1)*(t+2)/2 >= n  and  t is minimal.          */
    idx_t t = 0;
    while ((t + 1) * (t + 2) / 2 < n)
        ++t;

    /* Number of entries before this antidiagonal.            */
    idx_t prev = t * (t + 1) / 2;

    /* Position inside the antidiagonal (0‑based).            */
    idx_t offset = n - prev - 1;

    /* Convert to row (r) and column (c) – both 1‑based.      */
    seq_t r = (seq_t)(offset + 1);
    seq_t c = (seq_t)(t + 1 - offset);

    /* First part of the closed form.                          */
    seq_t term1 = ((seq_t)2 * r + (seq_t)2 * c - 1) *
                  (c * ((seq_t)2 * c - 1));

    /* Second part – division by 3 is exact.                  */
    seq_t term2 = c * (c + 1) * ((seq_t)8 * c - 5) / 3;

    return term1 - term2;
}
