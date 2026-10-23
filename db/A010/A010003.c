/*
 * A010003  a(0) = 1, a(n) = 11*n^2 + 2 for n>0.
 * https://oeis.org/A010003
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(n) = 11*n^2 + 2 for n > 0
 * Range:     u8 n <= 4, u16 n <= 77, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    seq_t nn = (seq_t)n;
    return (nn * nn * (seq_t)11) + (seq_t)2;
}
