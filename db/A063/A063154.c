/*
 * A063154  Dimension of the space of weight 2n cusp forms for Gamma_0( 86 ).
 * https://oeis.org/A063154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10 for n = 1; a(n) = 22*n - 13 for n > 1
 * Range:     u8 n <= 12, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)10;
    /* For n>1 use the linear formula 22*n-13 */
    return (seq_t)n * (seq_t)22 - (seq_t)13;
}
