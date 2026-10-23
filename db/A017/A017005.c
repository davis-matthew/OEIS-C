/*
 * A017005  a(n) = 7n + 2.
 * https://oeis.org/A017005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*n + 2
 * Range:     u8 n <= 36, u16 n <= 1120, u32 n <= 1120, u64 n <= 1120,
 *            u128 n <= 1120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)7 * (seq_t)n + (seq_t)2;
}
