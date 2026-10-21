/*
 * A063152  Dimension of the space of weight 2n cusp forms for Gamma_0( 84 ).
 * https://oeis.org/A063152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11 for n = 1; a(n) = 32 * n - 22 for n > 1
 * Range:     u8 n <= 8, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return 11;
    return (seq_t)n * 32 - 22;
}
