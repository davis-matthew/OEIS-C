/*
 * A027575  a(n) = n^2 + (n+1)^2 + (n+2)^2 + (n+3)^2.
 * https://oeis.org/A027575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n^2 + 12*n + 14
 * Range:     u8 n <= 6, u16 n <= 126, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n_sq = nn * nn;
    return 4 * n_sq + 12 * nn + (seq_t)14;
}
