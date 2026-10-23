/*
 * A154105  a(n) = 12*n^2 + 18*n + 7.
 * https://oeis.org/A154105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*n^2 + 18*n + 7
 * Range:     u8 n <= 3, u16 n <= 73, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return (seq_t)12 * nn * nn + (seq_t)18 * nn + (seq_t)7;
}
