/*
 * A017152  a(n) = (8*n + 7)^4.
 * https://oeis.org/A017152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n + 7)^4
 * Range:     u8 none, u16 n <= 1, u32 n <= 31, u64 n <= 8191, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t b = (seq_t)n * 8 + 7;
    seq_t b2 = b * b;
    return b2 * b2;
}
