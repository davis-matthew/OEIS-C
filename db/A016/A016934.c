/*
 * A016934  a(n) = (6*n + 2)^2.
 * https://oeis.org/A016934
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n + 2)^2
 * Range:     u8 n <= 2, u16 n <= 42, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = 2 + 6 * (seq_t)n;
    return m * m;
}
