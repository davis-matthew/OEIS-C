/*
 * A047407  Numbers that are congruent to {0, 4, 6} mod 8.
 * https://oeis.org/A047407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(n - 1 + floor((n + 1)/3))
 * Range:     u8 n <= 96, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t term = 2 * (nn - 1 + (nn + 1) / 3);
    return term;
}
