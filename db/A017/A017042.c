/*
 * A017042  a(n) = (7*n + 5)^2.
 * https://oeis.org/A017042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n + 5)^2
 * Range:     u8 n <= 1, u16 n <= 35, u32 n <= 9361, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t  = nn * 7 + 5;
    return t * t;
}
