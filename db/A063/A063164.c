/*
 * A063164  Dimension of the space of weight 2n cusp forms for Gamma_0( 96 ).
 * https://oeis.org/A063164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=9, a(n)=8*(4*n-3) for n>1
 * Range:     u8 n <= 8, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)9;
    if (n == 0) return (seq_t)0;  /* undefined for n<1, return 0 */
    // Compute 8*(4*n - 3) safely using seq_t arithmetic
    seq_t nn = (seq_t)n;
    return nn * (seq_t)32 - (seq_t)24;   /* 32*n - 24 == 8*(4*n-3) */
}
