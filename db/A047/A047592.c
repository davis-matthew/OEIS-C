/*
 * A047592  Numbers that are congruent to {1, 2, 3, 4, 5, 6, 7} mod 8.
 * https://oeis.org/A047592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - 1 + floor((n+6)/7)
 * Range:     u8 n <= 224, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n >= 1 */
    seq_t nn = (seq_t)n;
    return nn - 1 + (nn + 6) / 7;
}
