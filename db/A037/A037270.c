/*
 * A037270  a(n) = n^2*(n^2 + 1)/2.
 * https://oeis.org/A037270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n^2*(n^2+1)/2
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 269, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nsq = (seq_t)n * (seq_t)n;
    seq_t inc = nsq + 1;
    seq_t prod = nsq * inc;
    return prod / 2;
}
