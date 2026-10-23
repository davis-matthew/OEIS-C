/*
 * A017260  a(n) = (9*n + 8)^4.
 * https://oeis.org/A017260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n + 8)^4
 * Range:     u8 none, u16 n <= 0, u32 n <= 27, u64 n <= 7280, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    seq_t t  = (tn * 9) + 8;
    seq_t t2 = t * t;
    return t2 * t2;
}
