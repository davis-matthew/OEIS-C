/*
 * A017368  a(n) = (10*n + 8)^4.
 * https://oeis.org/A017368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 8)^4
 * Range:     u8 none, u16 n <= 0, u32 n <= 24, u64 n <= 6552, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)10 * (seq_t)n + 8;
    t = t * t; /* square */
    t = t * t; /* fourth power */
    return t;
}
