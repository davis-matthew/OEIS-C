/*
 * A047252  Numbers that are congruent to {0, 1, 3, 4, 5} mod 6.
 * https://oeis.org/A047252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((6*n - 3)/5)
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* for n == 0 the offset of the sequence is undefined, return 0 */
    if (n == 0) return 0;
    return ((seq_t)6 * n - 3) / 5;
}
