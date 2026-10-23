/*
 * A145980  a(n) = 29 + 73*n + 37*n^2.
 * https://oeis.org/A145980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 29 + 73*n + 37*n^2
 * Range:     u8 n <= 1, u16 n <= 41, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return (seq_t)29 + (seq_t)73 * nn + (seq_t)37 * nn * nn;
}
