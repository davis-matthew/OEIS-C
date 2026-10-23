/*
 * A033596  a(n) = (n^2 - 1)*(n^2 - 3).
 * https://oeis.org/A033596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 - 1)*(n^2 - 3)
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 256, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)0;
    seq_t nsq = (seq_t)n * (seq_t)n;
    return (nsq - (seq_t)1) * (nsq - (seq_t)3);
}
