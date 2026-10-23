/*
 * A047332  Numbers that are congruent to {0, 2, 3, 5, 6} mod 7.
 * https://oeis.org/A047332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((7*n-3)/5)
 * Range:     u8 n <= 183, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return ((seq_t)7 * (seq_t)n - (seq_t)3) / (seq_t)5;
}
