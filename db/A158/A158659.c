/*
 * A158659  a(n) = 784*n^2 + 28.
 * https://oeis.org/A158659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 784*n^2 + 28
 * Range:     u8 n <= 0, u16 n <= 9, u32 n <= 2340, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return (nn * nn * (seq_t)784) + (seq_t)28;
}
