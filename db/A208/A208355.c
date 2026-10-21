/*
 * A208355  Right edge of the triangle in A208101.
 * https://oeis.org/A208355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Catalan(floor((n+1)/2))
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 38, u64 n <= 72, u128 n <= 138,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A208355. */
seq_t a(idx_t n)
{
    /* floor((n+1)/2) for unsigned indices */
    idx_t k = (n + 1) >> 1;

    /* Catalan numbers via recurrence
       C_0 = 1
       C_m = C_{m-1} * 2*(2m-1) / (m+1)  (integer division)   */
    seq_t catalan = 1;                 /* C_0 */
    for (idx_t m = 1; m <= k; ++m)
    {
        /* Compute numerator = 2*(2*m-1) and denominator = m+1 */
        seq_t numerator   = (seq_t)2 * ((seq_t)2 * m - 1); /* 2*(2m-1) */
        seq_t denominator = m + 1;

        /* Perform the multiplication and division without overflow.
           Since (r * numerator) < denominator * numerator <=
           (2 * (2k-1)) * (k+1) <= ~4e8 for k <= 10000, the product
           fits in seq_t.  The final result fits in seq_t when the
           Catalan number does. */
        seq_t quotient = catalan / denominator;
        seq_t remainder = catalan % denominator;
        catalan = quotient * numerator
                  + (remainder * numerator) / denominator;
    }
    return catalan;
}
