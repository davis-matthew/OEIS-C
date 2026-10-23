/*
 * A017487  a(n) = (11*n + 8)^3.
 * https://oeis.org/A017487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 8)^3
 * Range:     u8 none, u16 n <= 2, u32 n <= 147, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t t = (x * 11) + 8;
    seq_t r = t * t * t;
    return r;
}
