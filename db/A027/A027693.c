/*
 * A027693  a(n) = n^2 + n + 8.
 * https://oeis.org/A027693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*n + n + 8
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 4000, u64 n <= 4000,
 *            u128 n <= 4000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t un = (seq_t)n;          /* cast index to seq_t first */
    return un * un + un + (seq_t)8;
}
