/*
 * A047209  Numbers that are congruent to {1, 4} mod 5.
 * https://oeis.org/A047209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((5*n-2)/2)
 * Range:     u8 n <= 102, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is assumed to be ≥ 1, as specified by the offset 1. */
    idx_t temp = 5ULL * n - 2ULL;
    return (seq_t)(temp / 2ULL);
}
