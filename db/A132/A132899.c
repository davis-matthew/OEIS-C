/*
 * A132899  Row sums of triangle A132898.
 * https://oeis.org/A132899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^(n-1)*(n^2 + ceil(n/2)) - n
 * Range:     i8 n <= 11, i16 n <= 181, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n * (seq_t)n;
    seq_t halfceil = (seq_t)((n + 1ULL) / 2ULL);
    seq_t s = nn + halfceil;
    seq_t term = (n & 1ULL) ? s : -s;
    return term - (seq_t)n;
}
