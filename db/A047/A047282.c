/*
 * A047282  Numbers that are congruent to {1, 3, 6} mod 7.
 * https://oeis.org/A047282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n - 1 + floor(n/3) (OEIS A047282)
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n >= 1 (offset 1).  For n == 0 return 0 as a safe default. */
    if (n == 0) return 0;

    seq_t nn = (seq_t)n;
    return 2 * nn - 1 + nn / 3;
}
