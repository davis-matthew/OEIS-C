/*
 * A017402  a(n) = (11*n+1)^2.
 * https://oeis.org/A017402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 1)^2
 * Range:     u8 n <= 1, u16 n <= 23, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)11 * (seq_t)n + 1;
    return t * t;
}
