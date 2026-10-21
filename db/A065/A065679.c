/*
 * A065679  If n is even, a(n) = n^2 else a(n) = n.
 * https://oeis.org/A065679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n if n is odd, otherwise n^2
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1) == 0) {
        return (seq_t)n * (seq_t)n;
    } else {
        return (seq_t)n;
    }
}
