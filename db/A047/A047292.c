/*
 * A047292  Numbers that are congruent to {2, 4, 6} mod 7.
 * https://oeis.org/A047292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((7*n - 1) / 3)
 * Range:     u8 n <= 109, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    return (((seq_t)n * 7) - 1) / 3;
}
