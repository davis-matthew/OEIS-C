/*
 * A169610  Numbers that are congruent to {5, 30} mod 37.
 * https://oeis.org/A169610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (74*n + 13*(-1)^n - 41)/4
 * Range:     u8 n <= 3, u16 n <= 959, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* offset is 1; for n == 0 return 0 (ignored by tests) */
    if (n == 0) return 0;
    seq_t val = (seq_t)n * 74;
    if ((n & 1) == 0)          /* n even -> +13 */
        val += 13;
    else                       /* n odd -> -13 */
        val -= 13;
    val -= 41;
    return val / 4;
}
