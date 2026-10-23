/*
 * A016768  a(n) = (3*n)^4.
 * https://oeis.org/A016768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n)^4
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 85, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)3 * (seq_t)n;
    t = t * t;
    t = t * t;
    return t;
}
