/*
 * A010133  Continued fraction for sqrt(41).
 * https://oeis.org/A010133
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 20000, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 86 terms through n = 8795 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A010133: Continued fraction for sqrt(41)
   a(0) = 6
   for n > 0: a(n) = 12 if n % 3 == 0 else 2
*/

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)6;
    }
    return (n % 3 == 0) ? (seq_t)12 : (seq_t)2;
}
