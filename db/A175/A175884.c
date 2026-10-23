/*
 * A175884  Numbers that are congruent to {0, 2, 4, 7, 9} mod 12.
 * https://oeis.org/A175884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(12*(n-1)/5)
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n ≥ 1 (offset 1) */
    return (seq_t)(((n - (idx_t)1) * (idx_t)12) / (idx_t)5);
}
