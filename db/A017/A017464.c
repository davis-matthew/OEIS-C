/*
 * A017464  a(n) = (11*n + 6)^4.
 * https://oeis.org/A017464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 6)^4
 * Range:     u8 none, u16 n <= 0, u32 n <= 22, u64 n <= 5957, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n;
    t = 11 * t + 6;
    seq_t r = t * t;
    r = r * r;
    return r;
}
