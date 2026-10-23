/*
 * A158187  a(n) = 10*n^2 + 1.
 * https://oeis.org/A158187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*n^2 + 1
 * Range:     u8 n <= 5, u16 n <= 80, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t nsq = nn * nn;
    return nsq * (seq_t)10 + (seq_t)1;
}
