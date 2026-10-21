/*
 * A063160  Dimension of the space of weight 2n cusp forms for Gamma_0( 92 ).
 * https://oeis.org/A063160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 24*n - 15 for n > 1 (a(1) = 10)
 * Range:     u8 n <= 11, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)10;

    /* Compute 24*n - 15 using seq_t arithmetic only */
    return (seq_t)n * ((seq_t)24) - (seq_t)15;
}
