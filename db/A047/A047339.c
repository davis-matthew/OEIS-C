/*
 * A047339  Numbers that are congruent to {2, 3, 4} mod 7.
 * https://oeis.org/A047339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*floor((n-1)/3) + ((n-1) mod 3) + 2 (offset 1)
 * Range:     u8 n <= 110, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1‑based.  For n >= 1, the expression is well defined. */
    idx_t q = (n - 1) / 3;   /* quotient of (n-1) by 3 */
    idx_t r = (n - 1) % 3;   /* remainder of (n-1) modulo 3 */
    return (seq_t)7 * q + (seq_t)r + 2;
}
