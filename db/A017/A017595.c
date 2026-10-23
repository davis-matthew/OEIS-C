/*
 * A017595  a(n) = (12*n + 6)^3.
 * https://oeis.org/A017595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 6)^3
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 134, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n * 12 + 6;
    return t * t * t;
}
