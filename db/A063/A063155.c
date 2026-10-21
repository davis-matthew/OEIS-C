/*
 * A063155  Dimension of the space of weight 2n cusp forms for Gamma_0( 87 ).
 * https://oeis.org/A063155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 9; for n > 1, a(n) = 4*(5*n - 3) = 20*n - 12
 * Range:     u8 n <= 13, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)                /* first term */
        return (seq_t)9;
    if (n == 0)                /* safety for unexpected zero index */
        return (seq_t)0;
    return (seq_t)20 * (seq_t)n - (seq_t)12;
}
