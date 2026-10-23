/*
 * A175887  Numbers that are congruent to {1, 14} mod 15.
 * https://oeis.org/A175887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (30*n+11*(-1)^n-15)/4
 * Range:     u8 n <= 34, u16 n <= 8738, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t num;
    if (n & 1)       /* n odd: (-1)^n = -1 */
        num = 30*n - 11 - 15;
    else            /* n even: (-1)^n = 1 */
        num = 30*n + 11 - 15;
    return (seq_t)(num / 4);
}
