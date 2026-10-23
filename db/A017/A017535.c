/*
 * A017535  a(n) = (12*n+1)^3.
 * https://oeis.org/A017535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n+1)^3
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 135, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)12 * (seq_t)n + (seq_t)1;
    return m * m * m;
}
