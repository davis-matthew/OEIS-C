/*
 * A005919  Number of points on surface of tricapped prism: a(n) = 7*n^2 + 2
 *          for n > 0, a(0)=1.
 * https://oeis.org/A005919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*n^2 + 2 for n > 0, a(0) = 1
 * Range:     u8 n <= 6, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    seq_t sn = (seq_t)n;
    return 7 * sn * sn + 2;
}
