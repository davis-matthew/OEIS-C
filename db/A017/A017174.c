/*
 * A017174  a(n) = (9*n + 1)^2.
 * https://oeis.org/A017174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n + 1)^2
 * Range:     u8 n <= 1, u16 n <= 28, u32 n <= 7281, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)9 * (seq_t)n + 1;
    return t * t;
}
