/*
 * A016959  a(n) = (6*n + 4)^3.
 * https://oeis.org/A016959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n + 4)^3
 * Range:     u8 n <= 0, u16 n <= 6, u32 n <= 270, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n * 6 + 4;
    return m * m * m;
}
