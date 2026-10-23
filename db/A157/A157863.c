/*
 * A157863  a(n) = 103680000*n^2 + 28800*n + 1.
 * https://oeis.org/A157863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 103680000*n^2 + 28800*n + 1
 * Range:     u8 none, u16 none, u32 n <= 6, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return (seq_t)103680000 * nn * nn + (seq_t)28800 * nn + (seq_t)1;
}
