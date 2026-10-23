/*
 * A027468  9 times the triangular numbers A000217.
 * https://oeis.org/A027468
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*n*(n+1)/2
 * Range:     u8 n <= 7, u16 n <= 120, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t tri = nn * (nn + (seq_t)1) / (seq_t)2;
    return tri * (seq_t)9;
}
