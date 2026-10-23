/*
 * A168199  a(n) = 3*n - a(n-1) + 1 with n > 1, a(1)=2.
 * https://oeis.org/A168199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n + 5 + 3*(-1)^n)/4
 * Range:     u8 n <= 41, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t numerator;
    if (n & 1)              /* n odd: (-1)^n = -1  */
        numerator = 6 * sn + 2;   /* 6n + 5 - 3 = 6n + 2 */
    else
        numerator = 6 * sn + 8;   /* 6n + 5 + 3 = 6n + 8 */
    return numerator / (seq_t)4;
}
