/*
 * A063116  Dimension of the space of weight 2n cusp forms for Gamma_0( 48 ).
 * https://oeis.org/A063116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=3; for n>1: a(n)=2*(8*n-7)
 * Range:     u8 n <= 16, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1u)   /* offset 1: first term is 3 */
        return (seq_t)3U;
    seq_t sn = (seq_t)n;          /* convert index to seq_t */
    return 2U * (8U * sn - 7U);
}
