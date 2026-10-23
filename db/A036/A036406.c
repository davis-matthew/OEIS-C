/*
 * A036406  a(n) = ceiling(n^2/8).
 * https://oeis.org/A036406
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n^2/8)
 * Range:     u8 n <= 15, u16 n <= 723, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast the index to seq_t before any arithmetic to avoid unexpected
       integer promotion rules.  Then compute n*n, add 7, and divide
       by 8 to perform the ceiling operation. */
    seq_t nn = (seq_t)n;
    seq_t sq = nn * nn;
    return (sq + 7) / 8;
}
