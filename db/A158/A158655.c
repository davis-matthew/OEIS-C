/*
 * A158655  a(n) = 729*n^2 - 27.
 * https://oeis.org/A158655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 729*n^2 - 27
 * Range:     u8 none, u16 n <= 9, u32 n <= 2427, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t nn = (seq_t)n * (seq_t)n;
    return (seq_t)729 * nn - (seq_t)27;
}
