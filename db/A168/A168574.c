/*
 * A168574  a(n) = (4*n + 3)*(1 + 2*n^2)/3.
 * https://oeis.org/A168574
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n + 3)*(1 + 2*n^2)/3
 * Range:     u8 n <= 4, u16 n <= 28, u32 n <= 1170, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn   = (seq_t)n;
    seq_t t1   = sn * 4 + 3;
    seq_t t2   = sn * sn * 2 + 1;
    return (t1 * t2) / 3;
}
