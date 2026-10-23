/*
 * A185732  Accumulation array of the polygonal number array (A086270), by
 *          antidiagonals.
 * https://oeis.org/A185732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the accumulation array entry of the polygonal number *
 *            array (A086270) read by antidiagonals. * For the diagonal index
 *            d (smallest integer with d(d+1)/2 ≥ n), * let i = n - d(d-1)/2
 *            (row index, 1‑based) * j = d + 1 - i (column index, 1‑based) *
 *            Then * A(i,j) = C(j+1,3) * i(i+1)/2 + C(j+1,...
 * Range:     u8 n <= 30, u16 n <= 388, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find diagonal d such that d(d+1)/2 ≥ n */
    idx_t d = 1;
    while ((d * (d + 1) / 2) < n) {
        ++d;
    }

    /* Coordinates (i,j) on this diagonal */
    idx_t offset = (d - 1) * d / 2;
    idx_t i = n - offset;              /* row (≥1) */
    idx_t j = d + 1 - i;               /* column (≥1) */

    /* Binomial coefficients C(j+1,3) and C(j+1,2) */
    seq_t c3 = (seq_t)(j + 1) * j * (j - 1) / 6;   /* C(j+1,3) */
    seq_t c2 = (seq_t)(j + 1) * j / 2;             /* C(j+1,2) */

    /* i(i+1)/2 */
    seq_t tmp = (seq_t)i * (i + 1) / 2;

    /* Compute A(i,j) */
    seq_t result = c3 * tmp + c2 * (seq_t)i;
    return result;
}
