/*
 * A158056  a(n) = 16*n^2 + 2*n.
 * https://oeis.org/A158056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*n^2 + 2*n
 * Range:     u8 n <= 3, u16 n <= 63, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sq = (seq_t)n;
    /* Compute 16*n*n + 2*n using only integer arithmetic */
    return (sq * sq * (seq_t)16) + (sq * (seq_t)2);
}
