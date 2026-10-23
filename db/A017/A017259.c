/*
 * A017259  a(n) = (9*n + 8)^3.
 * https://oeis.org/A017259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n + 8)^3
 * Range:     u8 none, u16 n <= 3, u32 n <= 179, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n * 9 + 8;
    return m * m * m;
}
