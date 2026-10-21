/*
 * A063150  Dimension of the space of weight 2n cusp forms for Gamma_0( 82 ).
 * https://oeis.org/A063150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=9, for n>1: a(n)=21*n-13 when n is odd, a(n)=21*n-12 when n
 *            is even
 * Range:     u8 n <= 12, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* unused, offset is 1 */
    if (n == 1) return 9;
    if (n & 1) {
        return (seq_t)(21u * (seq_t)n - (seq_t)13);
    } else {
        return (seq_t)(21u * (seq_t)n - (seq_t)12);
    }
}
