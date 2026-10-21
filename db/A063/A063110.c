/*
 * A063110  Dimension of the space of weight 2n cusp forms for Gamma_0(42).
 * https://oeis.org/A063110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=5, a(n)=16*n-12 for n>1
 * Range:     u8 n <= 16, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)5;
    return (seq_t)n * (seq_t)16 - (seq_t)12;
}
