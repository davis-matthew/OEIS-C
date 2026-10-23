/*
 * A047379  Numbers that are congruent to {0, 2, 4, 5} mod 7.
 * https://oeis.org/A047379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((7*n - 5)/4)
 * Range:     u8 n <= 37, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = (seq_t)7 * (seq_t)n - 5;
    return val / 4;
}
