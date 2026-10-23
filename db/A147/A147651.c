/*
 * A147651  First trisection of A028560.
 * https://oeis.org/A147651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*n*(n+2)
 * Range:     u8 n <= 4, u16 n <= 84, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* cast to the sequence type */
    return (seq_t)9 * nn * (nn + (seq_t)2);
}
