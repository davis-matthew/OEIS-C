/*
 * A124093  Triangular numbers alternating with squares.
 * https://oeis.org/A124093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+2)/8 if n is even; a(n) = (n-1)^2/4 if n is odd
 * Range:     u8 n <= 32, u16 n <= 512, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    if ((n & 1) == 0) {
        return (x * (x + 2)) / (seq_t)8;
    } else {
        seq_t y = x - (seq_t)1;
        return (y * y) / (seq_t)4;
    }
}
