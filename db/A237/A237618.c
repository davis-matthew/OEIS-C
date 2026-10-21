/*
 * A237618  a(n) = n*(n + 1)*(19*n - 16)/6.
 * https://oeis.org/A237618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(19*n-16)/6
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 713, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t numerator = nn * (nn + 1) * (19 * nn - 16);
    return numerator / 6;
}
