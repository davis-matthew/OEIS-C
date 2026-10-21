/*
 * A063168  Dimension of the space of weight 2n cusp forms for Gamma_0( 100 ).
 * https://oeis.org/A063168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=7, a(2)=36, a(n)=a(n-1)+30, n>2
 * Range:     u8 n <= 9, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 7;
    if (n == 2) return 36;
    /* for n >= 3: a(n)=36 + 30*(n-2) */
    return 30 * (((seq_t)n) - 2) + 36;
}
