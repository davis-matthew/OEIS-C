/*
 * A010005  a(0) = 1, a(n) = 15*n^2 + 2 for n>0.
 * https://oeis.org/A010005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for n = 0, otherwise a(n) = 15*n^2 + 2
 * Range:     u8 n <= 4, u16 n <= 66, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t nn = (seq_t)n;
    return (seq_t)15 * nn * nn + (seq_t)2;
}
