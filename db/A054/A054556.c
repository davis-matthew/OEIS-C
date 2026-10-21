/*
 * A054556  a(n) = 4*n^2 - 9*n + 6.
 * https://oeis.org/A054556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n^2 - 9*n + 6
 * Range:     u8 n <= 9, u16 n <= 129, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t rn = (seq_t)n;
    seq_t res = rn * rn;   /* n^2 */
    res = res * 4;         /* 4*n^2 */
    res = res + 6;         /* 4*n^2 + 6 */
    res = res - rn * 9;    /* 4*n^2 + 6 - 9*n = 4*n^2 - 9*n + 6 */
    return res;
}
