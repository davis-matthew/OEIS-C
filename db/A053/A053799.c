/*
 * A053799  Number of basis partitions of n+9 with Durfee square size 3.
 * https://oeis.org/A053799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*(1 + floor(n^2/3)) for n > 0, a(0) = 1
 * Range:     u8 n <= 15, u16 n <= 300, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    seq_t sq = (seq_t)n * (seq_t)n;
    seq_t f  = sq / 3;
    return 2 * (1 + f);
}
