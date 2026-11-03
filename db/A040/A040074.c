/*
 * A040074  Continued fraction for sqrt(84).
 * https://oeis.org/A040074
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 20000, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 82 terms through n = 9978 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Continued fraction for sqrt(84).
   a(0) = 9;
   for n >= 1: a(n) = 6 if n is odd, 18 if n is even. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)9;
    return ((n & (idx_t)1) ? (seq_t)6 : (seq_t)18);
}
