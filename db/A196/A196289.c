/*
 * A196289  a(n) = n^9 - n.
 * https://oeis.org/A196289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = n^9 - n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 11, u64 n <= 138,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t r = x * x * x * x * x * x * x * x * x; /* x^9 */
    return r - x;
}
