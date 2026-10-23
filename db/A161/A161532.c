/*
 * A161532  a(n) = 2*n^2 + 8*n + 1.
 * https://oeis.org/A161532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*n^2 + 8*n + 1
 * Range:     u8 n <= 9, u16 n <= 179, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return 2 * nn * nn + 8 * nn + 1;
}
