/*
 * A172082  a(n) = n*(n+1)*(6*n-5)/2.
 * https://oeis.org/A172082
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(6*n-5)/2
 * Range:     u8 n <= 4, u16 n <= 27, u32 n <= 940, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_t = (seq_t)n;
    seq_t part1 = n_t * (n_t + 1);
    seq_t part2 = (seq_t)6 * n_t - (seq_t)5;
    return (part1 * part2) / (seq_t)2;
}
