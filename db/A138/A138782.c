/*
 * A138782  a(n) = n*(3*n-1)*n!/2.
 * https://oeis.org/A138782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(3*n-1)*n!/2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n! */
    seq_t fact = 1;
    for (idx_t i = 1; i <= n; ++i) {
        fact *= (seq_t)i;
    }

    /* Compute the coefficient n*(3*n-1)/2 */
    seq_t coeff = (seq_t)n * (seq_t)(3 * n - 1);
    coeff /= 2;  /* n*(3*n-1) is always even */

    /* Return n! * coeff */
    return fact * coeff;
}
