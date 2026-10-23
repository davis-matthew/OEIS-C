/*
 * A017176  a(n) = (9n+1)^4.
 * https://oeis.org/A017176
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n+1)^4
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)9 * n + 1;
    return t * t * t * t;
}
