/*
 * A008959  Final digit of squares: a(n) = n^2 mod 10.
 * https://oeis.org/A008959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = (n*n) mod 10
 * Range:     u8 n <= 271, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 10 to avoid overflow, then square and mod 10 */
    seq_t d = (seq_t)n % (seq_t)10;
    return (d * d) % (seq_t)10;
}
