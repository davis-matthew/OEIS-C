/*
 * A063151  Dimension of the space of weight 2n cusp forms for Gamma_0( 83 ).
 * https://oeis.org/A063151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7 for n = 1; a(n) = 14*n - 8 for n > 1
 * Range:     u8 n <= 18, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)7;
    return (seq_t)n * (seq_t)14 - (seq_t)8;
}
