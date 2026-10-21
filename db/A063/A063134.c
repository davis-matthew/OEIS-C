/*
 * A063134  Dimension of the space of weight 2n cusp forms for Gamma_0( 66 ).
 * https://oeis.org/A063134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 9, a(n) = 8 * (3 * n - 2) for n > 1
 * Range:     u8 n <= 11, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)9;
    seq_t n_t = (seq_t)n;
    /* Compute 8 * (3 * n - 2) */
    return (seq_t)8 * ((seq_t)3 * n_t - (seq_t)2);
}
