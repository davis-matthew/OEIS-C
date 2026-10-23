/*
 * A033951  Write 1,2,... in a clockwise spiral; sequence gives numbers on
 *          positive x axis.
 * https://oeis.org/A033951
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 4*n^2 + 3*n + 1
 * Range:     u8 n <= 7, u16 n <= 127, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    return (seq_t)4 * sn * sn + (seq_t)3 * sn + (seq_t)1;
}
