/*
 * A047369  Numbers that are congruent to {1, 2, 3, 4, 5} mod 7.
 * https://oeis.org/A047369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + 2 * floor((n-1)/5)
 * Range:     u8 n <= 183, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Offset 1: a(1) = 1 */
    if (n == 0) return 0;           /* undefined for n<1, return 0 */
    seq_t k = (seq_t)((n - 1) / 5); /* floor((n-1)/5) */
    return (seq_t)n + (k << 1);     /* n + 2*k */
}
