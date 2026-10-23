/*
 * A109118  a(n) = 2*(n^2 + 3*n + 1)^3.
 * https://oeis.org/A109118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(n^2 + 3*n + 1)^3
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n;
    seq_t s = t * t + (seq_t)3 * t + (seq_t)1;
    seq_t sq = s * s;
    seq_t res = sq * s;
    res *= (seq_t)2;
    return res;
}
