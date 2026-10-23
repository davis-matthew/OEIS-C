/*
 * A047398  Numbers that are congruent to {3, 6} mod 8.
 * https://oeis.org/A047398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n + (n%2) - 2
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset starts at 1, 0 is undefined */
    return (seq_t)4 * n + (seq_t)(n & (idx_t)1) - (seq_t)2;
}
