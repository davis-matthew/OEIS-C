/*
 * A027692  a(n) = n^2 + n + 7.
 * https://oeis.org/A027692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 + n + 7
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return nn * nn + nn + (seq_t)7;
}
