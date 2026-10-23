/*
 * A033547  Otto Haxel's guess for magic numbers of nuclear shells.
 * https://oeis.org/A033547
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = n*(n^2 + 5)/3
 * Range:     u8 n <= 8, u16 n <= 58, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return nn * (nn * nn + 5) / 3;
}
