/*
 * A000326  Pentagonal numbers: a(n) = n*(3*n-1)/2.
 * https://oeis.org/A000326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  core, nonn, easy, nice
 * Formula:   a(n) = n*(3*n-1)/2
 * Range:     u8 n <= 13, u16 n <= 209, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;
    seq_t t = (seq_t)3 * m - 1;
    if (n % 2 == 0)
        return (m / 2) * t;
    return m * (t / 2);
}
