/*
 * A017452  a(n) = (11*n + 5)^4.
 * https://oeis.org/A017452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 5)^4
 * Range:     u8 none, u16 n <= 0, u32 n <= 22, u64 n <= 5957, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)11 * (seq_t)n + (seq_t)5;
    seq_t t2 = t * t;
    seq_t t4 = t2 * t2;
    return t4;
}
