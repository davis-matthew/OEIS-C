/*
 * A047451  Numbers that are congruent to {0, 6} mod 8.
 * https://oeis.org/A047451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n - 3 + (-1)^n
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 4 * (seq_t)n;
    if (n & 1)           /* n is odd: (-1)^n = -1, subtract 4 */
        val -= 4;
    else                 /* n is even: (-1)^n =  1, subtract 2 */
        val -= 2;
    return val;
}
