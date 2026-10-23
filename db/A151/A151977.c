/*
 * A151977  Numbers that are congruent to {0, 1} mod 16.
 * https://oeis.org/A151977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (32*n + ((n mod 2) ? 14 : -14) - 46) / 4
 * Range:     u8 n <= 8, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t temp = (seq_t)n * (seq_t)32;
    seq_t num;
    if ((n & 1) != 0) {
        /* n is odd: 32*n - 32 */
        num = temp - (seq_t)32;
    } else {
        /* n is even: 32*n - 60 */
        num = temp - (seq_t)60;
    }
    return num / (seq_t)4;
}
