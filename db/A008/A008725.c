/*
 * A008725  Molien series for 3-dimensional group [2,n] = *22n.
 * https://oeis.org/A008725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+4)*(n+5)/14)
 * Range:     u8 n <= 55, u16 n <= 953, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t r1 = (seq_t)n + 4;
    seq_t r2 = (seq_t)n + 5;
    return r1 * r2 / 14;
}
