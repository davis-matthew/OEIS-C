/*
 * A084570  Partial sums of A084263.
 * https://oeis.org/A084570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ceil((n^3 + 3 n^2 + 5 n + 3) / 6)
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = n;
    seq_t num = nn * nn * nn + 3 * nn * nn + 5 * nn + 3;
    return (num + 5) / 6;
}
