/*
 * A168240  a(n) = 13*n^2 + 7*n + 1.
 * https://oeis.org/A168240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13*n^2 + 7*n + 1
 * Range:     u8 n <= 4, u16 n <= 70, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t res = sn * sn * (seq_t)13;   /* 13*n^2 */
    res += sn * (seq_t)7;              /* + 7*n */
    res += (seq_t)1;                   /* + 1 */
    return res;
}
