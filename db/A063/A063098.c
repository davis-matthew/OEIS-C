/*
 * A063098  Dimension of the space of weight 2n cusp forms for Gamma_0( 30 ).
 * https://oeis.org/A063098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 3, a(n) = 12*n - 10 for n > 1
 * Range:     u8 n <= 22, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1u) {
        return (seq_t)3;
    }
    /* Compute the linear expression using seq_t arithmetic */
    return (seq_t)12 * (seq_t)n - (seq_t)10;
}
