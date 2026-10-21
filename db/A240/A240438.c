/*
 * A240438  Greatest minimal difference between numbers of adjacent cells in a
 *          regular hexagonal honeycomb of order n with cells numbered from 1
 *          through the total number of cells, the order n corresponding to
 *          the number of cells on one side of the honeycomb.
 * https://oeis.org/A240438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1) - floor((n+1)/3)
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;                     /* convert index to seq_t */
    seq_t prod   = n_seq * (n_seq - 1);          /* n*(n-1) */
    seq_t fdiv   = (seq_t)((n + 1) / 3);          /* floor((n+1)/3) */
    return prod - fdiv;
}
