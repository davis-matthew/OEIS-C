/*
 * A008732  Molien series for 3-dimensional group [2,n] = *22n.
 * https://oeis.org/A008732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) = floor((n+3)*(n+4)/10)
 * Range:     u8 n <= 47, u16 n <= 806, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return ((nn + (seq_t)3) * (nn + (seq_t)4)) / (seq_t)10;
}
