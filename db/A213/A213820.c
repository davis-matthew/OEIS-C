/*
 * A213820  Principal diagonal of the convolution array A213819.
 * https://oeis.org/A213820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -n + n^2 + 2*n^3
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t n2 = sn * sn;      /* n^2 */
    seq_t n3 = n2 * sn;      /* n^3 */
    return (n3 * 2) + n2 - sn;
}
