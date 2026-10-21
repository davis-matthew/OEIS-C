/*
 * A063148  Dimension of the space of weight 2n cusp forms for Gamma_0( 80 ).
 * https://oeis.org/A063148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=7; for n>1, a(n)=6*(4*n-3)
 * Range:     u8 n <= 11, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return 7;
    /* Convert the index to seq_t to avoid integer promotion issues. */
    seq_t four_n = (seq_t)4 * n;
    seq_t val    = 6 * (four_n - 3);
    return val;
}
