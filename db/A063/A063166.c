/*
 * A063166  Dimension of the space of weight 2n cusp forms for Gamma_0( 98 ).
 * https://oeis.org/A063166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 28*n - 22 for n > 1, a(1) = 7
 * Range:     u8 n <= 9, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)7;
    /* for n >= 2 use the linear formula */
    return (seq_t)(28 * (seq_t)n - (seq_t)22);
}
