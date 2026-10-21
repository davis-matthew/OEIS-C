/*
 * A063158  Dimension of the space of weight 2n cusp forms for Gamma_0( 90 ).
 * https://oeis.org/A063158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11 for n = 1 * a(n) = 36*n - 26 for n > 1 (integer
 *            arithmetic on seq_t)
 * Range:     u8 n <= 7, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) 
        return (seq_t)11;
    /* For n > 1 compute 36*n - 26. Convert n to seq_t before arithmetic. */
    return ((seq_t)n * 36) - 26;
}
