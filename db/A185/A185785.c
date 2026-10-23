/*
 * A185785  Second accumulation array of A107985, by antidiagonals.
 * https://oeis.org/A185785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (i+j+2) * C(i+2,3) * C(j+2,3) / 4, where (i,j) are the
 *            indices of the n‑th entry of A107985 when its entries are listed
 *            in antidiagonal order
 * Range:     u8 n <= 7, u16 n <= 124, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)                 /* offset is 1; behaviour below offset not required */
        return 0;

    /* Find the antidiagonal (sum of indices) that contains the n‑th element. */
    idx_t s = 2;
    while (s * (s - 1) / 2 < n)
        ++s;

    /* Number of elements before the current diagonal */
    idx_t prev = (s - 1) * (s - 2) / 2;

    /* Offset within this diagonal (0‑based) */
    idx_t off  = n - prev - 1;

    idx_t i = off + 1;            /* row index, ≥1 */
    idx_t j = s - i;              /* column index, ≥1 */

    /* Convert to seq_t for arithmetic. */
    seq_t ii = (seq_t)i;
    seq_t jj = (seq_t)j;

    /* C(n+2,3) = (n+2)(n+1)n / 6  */
    seq_t tri_i = ii * (ii + 1) * (ii + 2) / 6;
    seq_t tri_j = jj * (jj + 1) * (jj + 2) / 6;

    /* Compute the final formula. */
    seq_t sum = (seq_t)(i + j + 2);
    seq_t prod = tri_i * tri_j;
    prod *= sum;
    prod /= 4;

    return prod;
}
