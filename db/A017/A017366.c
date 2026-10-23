/*
 * A017366  a(n) = (10*n + 8)^2.
 * https://oeis.org/A017366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 8)^2
 * Range:     u8 n <= 0, u16 n <= 24, u32 n <= 6552, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n;
    t = ((seq_t)10 * t) + 8;
    return t * t;
}
