/*
 * A045943  Triangular matchstick numbers: a(n) = 3*n*(n+1)/2.
 * https://oeis.org/A045943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n*(n+1)/2
 * Range:     u8 n <= 12, u16 n <= 208, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t res = nn * (nn + 1) / 2;
    res *= 3;
    return res;
}
