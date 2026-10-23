/*
 * A139757  a(n) = (n+1)*(2n+1)^2.
 * https://oeis.org/A139757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+1)*(2n+1)^2
 * Range:     u8 n <= 3, u16 n <= 24, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t twon1 = nn * 2 + 1;
    return (nn + 1) * twon1 * twon1;
}
