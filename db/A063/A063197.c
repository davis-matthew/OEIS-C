/*
 * A063197  Dimension of the space of weight 2n cuspidal newforms for Gamma_0(
 *          9 ).
 * https://oeis.org/A063197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-1 - floor((n-1)/2) + floor((n-1)/3)
 * Range:     u8 n <= 306, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset < 1, safe guard */
    seq_t m = (seq_t)(n - 1);       /* n-1 fits in idx_t, cast to seq_t */
    return m - (m / 2) + (m / 3);   /* integer division implements floor */
}
