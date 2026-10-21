/*
 * A063136  Dimension of the space of weight 2n cusp forms for Gamma_0( 68 ).
 * https://oeis.org/A063136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 7, a(n) = 6*(3*n-2) for n ≥ 2
 * Range:     u8 n <= 14, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)7;
    /* n ≥ 2 */
    seq_t sn = (seq_t)n;
    seq_t val = (seq_t)6 * (((seq_t)3 * sn) - (seq_t)2);
    return val;
}
