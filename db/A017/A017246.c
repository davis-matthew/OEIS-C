/*
 * A017246  a(n) = (9*n + 7)^2.
 * https://oeis.org/A017246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n + 7)^2
 * Range:     u8 n <= 0, u16 n <= 27, u32 n <= 7280, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t s = (seq_t)9 * (seq_t)n + 7;
    return s * s;
}
