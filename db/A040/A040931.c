/*
 * A040931  Continued fraction for sqrt(963).
 * https://oeis.org/A040931
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n): Continued fraction partial quotients for sqrt(963)
   a(0) = 31, a(1) = 31, for n ≥ 2:
   a(n) = 62 if n is even, otherwise 31 */
seq_t a(idx_t n)
{
    if (n == 0 || n == 1) return 31;
    return (n & 1) ? 31 : 62;
}
