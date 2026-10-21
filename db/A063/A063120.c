/*
 * A063120  Dimension of the space of weight 2n cusp forms for Gamma_0( 52 ).
 * https://oeis.org/A063120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 5; for n > 1, a(n) = 14*n - 10
 * Range:     u8 n <= 18, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)5;
    return (seq_t)14 * (seq_t)n - (seq_t)10;
}
