/*
 * A033816  a(n) = 2*n^2 + 3*n + 3.
 * https://oeis.org/A033816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*n^2 + 3*n + 3
 * Range:     u8 n <= 10, u16 n <= 180, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return (seq_t)2 * nn * nn + (seq_t)3 * nn + (seq_t)3;
}
