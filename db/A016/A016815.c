/*
 * A016815  a(n) = (4*n + 1)^3.
 * https://oeis.org/A016815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n + 1)^3
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t t = x * 4 + 1;
    return t * t * t;
}
