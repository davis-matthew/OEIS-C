/*
 * A047381  Numbers that are congruent to {0, 1, 2, 4, 5} mod 7.
 * https://oeis.org/A047381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((7/5)(n-1))
 * Range:     u8 n <= 183, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset<1, but safe */
    seq_t s = (seq_t)n - 1;          /* convert to seq_t first */
    return (s * 7) / 5;             /* integer division is floor for positives */
}
