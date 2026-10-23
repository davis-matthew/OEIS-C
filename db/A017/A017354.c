/*
 * A017354  a(n) = (10*n + 7)^2.
 * https://oeis.org/A017354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 7)^2
 * Range:     u8 n <= 0, u16 n <= 24, u32 n <= 6552, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t k = (seq_t)n;
    seq_t t = 10 * k + 7;
    return t * t;
}
