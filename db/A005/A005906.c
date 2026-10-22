/*
 * A005906  Truncated tetrahedral numbers: a(n) = (1/6)*(n+1)*(23*n^2 + 19*n +
 *          6).
 * https://oeis.org/A005906
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(23*n^2 + 19*n + 6)/6
 * Range:     u8 n <= 2, u16 n <= 25, u32 n <= 1037, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_cast = (seq_t)n;
    seq_t n_plus_1 = n_cast + 1;
    seq_t n_sq = n_cast * n_cast;
    seq_t quad = 23 * n_sq + 19 * n_cast + 6;
    return (n_plus_1 * quad) / 6;
}
