/*
 * A038303  Triangle whose (i,j)-th entry is binomial(i,j)*10^(i-j)*1^j.
 * https://oeis.org/A038303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j) * 10^(i-j) where i = largest integer with
 *            (i+1)(i+2)/2 > n and j = n - i(i+1)/2. For each i, 0 ≤ j ≤ i
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 54, u64 n <= 190, u128 n <= 741,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th term of A038303 using only seq_t arithmetic. */
seq_t a(idx_t n)
{
    /* Determine the row index i by solving the triangular inequality. */
    idx_t i = 0;
    while (((i + 1) * (i + 2)) / 2 <= n) {
        ++i;
    }

    /* Determine the column index j within that row. */
    idx_t j = n - (i * (i + 1)) / 2;

    /* Compute binomial(i, j) multiplicatively to avoid large intermediates. */
    seq_t bin = 1;
    for (idx_t k = 1; k <= j; ++k) {
        /* Multiply by (i - j + k) and divide by k.  Division is exact. */
        bin = bin * (i - j + k) / k;
    }

    /* Compute 10^(i-j) by repeated multiplication. */
    seq_t p10 = 1;
    for (idx_t k = 0; k < i - j; ++k) {
        p10 *= 10;
    }

    /* Return the product. */
    return bin * p10;
}
