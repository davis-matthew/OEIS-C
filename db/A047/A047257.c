/*
 * A047257  Numbers that are congruent to {4, 5} mod 6.
 * https://oeis.org/A047257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n - (-1)^n
 * Range:     u8 n <= 84, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = (seq_t)3 * n;
    if (n & 1)
        val += 1;
    else
        val -= 1;
    return val;
}
