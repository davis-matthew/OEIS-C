/*
 * A033638  Quarter-squares plus 1 (that is, a(n) = A002620(n) + 1).
 * https://oeis.org/A033638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor((n^2)/4) + 1
 * Range:     u8 n <= 15, u16 n <= 510, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t sq = nn * nn;
    return (sq / 4) + 1;
}
