/*
 * A010012  a(0) = 1, a(n) = 22*n^2 + 2 for n>0.
 * https://oeis.org/A010012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 22*n^2 + 2 for n>0, a(0)=1
 * Range:     u8 n <= 3, u16 n <= 54, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t sn = (seq_t)n;
    return 22 * sn * sn + (seq_t)2;
}
