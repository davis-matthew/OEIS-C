/*
 * A016862  a(n) = (5*n + 1)^2.
 * https://oeis.org/A016862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n + 1)^2
 * Range:     u8 n <= 2, u16 n <= 50, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n * 5 + 1;
    return t * t;
}
