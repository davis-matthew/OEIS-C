/*
 * A047233  Numbers that are congruent to {0, 4} mod 6.
 * https://oeis.org/A047233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n + (-1)^n - 5)/2 Equivalent: a(n) = 3*n - (n%2?3:2)
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n starts at 1 for the OEIS offset */
    seq_t val = (seq_t)3 * (seq_t)n;
    if (n & 1ull) {            /* n odd -> subtract 3 */
        val -= (seq_t)3;
    } else {                   /* n even -> subtract 2 */
        val -= (seq_t)2;
    }
    return val;
}
