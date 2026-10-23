/*
 * A131052  A131047 * A000012.
 * https://oeis.org/A131052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = sum_{m=j-1 step 2}^{i-1} C(i, m) where i = smallest
 *            integer with i*(i+1)/2 ≥ n (row of the triangle) and j = column
 *            index adjusted to match the parity of i (see code). This is the
 *            product of the lower‐triangular matrix A131047 with the all‑ones
 *            matrix A000012
 * Range:     u8 n <= 36, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* determine the row i containing the element */
    idx_t i = 1;
    while ((i * (i + 1) / 2) < n) {
        ++i;
    }

    /* linear start of that row */
    idx_t start = ((i - 1) * i) / 2 + 1;

    /* column index within the row (1‑based) */
    idx_t j = n - start + 1;

    /* the element is constant on consecutive pairs of columns;
       if i and j have different parity we move to the next column. */
    if (((i - j) & 1) != 0) {
        ++j;
    }

    /* compute the required sum of binomial coefficients.
       We iterate m = 0 .. i-1, building C(i,m) incrementally. */
    seq_t sum  = 0;
    seq_t binom = 1;           /* C(i,0) */
    idx_t parity = (i - 1) & 1;/* parity that matches i - 1 */

    for (idx_t m = 0; m < i; ++m) {
        if (((m & 1) == parity) && (m >= j - 1)) {
            sum += binom;
        }
        if (m + 1 < i) {
            binom = binom * (seq_t)(i - m) / (seq_t)(m + 1);
        }
    }
    return sum;
}
