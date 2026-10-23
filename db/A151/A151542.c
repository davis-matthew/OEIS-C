/*
 * A151542  Generalized pentagonal numbers: a(n) = 12*n + 3*n*(n-1)/2.
 * https://oeis.org/A151542
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*n*(n+7)/2
 * Range:     u8 n <= 10, u16 n <= 205, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;
    return (m * (m + 7) * 3) / 2;
}
