/*
 * A047838  a(n) = floor(n^2/2) - 1.
 * https://oeis.org/A047838
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n^2/2) - 1
 * Range:     u8 n <= 15, u16 n <= 361, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t sq = nn * nn;
    seq_t res = sq / 2;
    return res - 1;
}
