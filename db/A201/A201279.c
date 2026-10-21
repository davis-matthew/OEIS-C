/*
 * A201279  a(n) = 6*n^2 + 10*n + 5.
 * https://oeis.org/A201279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*n^2 + 10*n + 5
 * Range:     u8 n <= 5, u16 n <= 103, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return 6 * nn * nn + 10 * nn + 5;
}
