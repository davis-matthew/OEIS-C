/*
 * A153037  a(n) = 2*n^2 + 16*n + 23.
 * https://oeis.org/A153037
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n^2 + 16*n + 23
 * Range:     u8 n <= 7, u16 n <= 177, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* cast index to seq_t to avoid narrowing */
    return nn * nn * (seq_t)2 + nn * (seq_t)16 + (seq_t)23;
}
