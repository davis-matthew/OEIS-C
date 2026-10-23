/*
 * A017427  a(n) = (11*n + 3)^3.
 * https://oeis.org/A017427
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 3)^3
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 147, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)11 * n + (seq_t)3;
    return t * t * t;
}
