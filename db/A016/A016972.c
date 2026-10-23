/*
 * A016972  a(n) = (6*n + 5)^4.
 * https://oeis.org/A016972
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n + 5)^4
 * Range:     u8 none, u16 n <= 1, u32 n <= 41, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t  = (seq_t)6 * n + 5;
    seq_t t2 = t * t;
    return t2 * t2;
}
