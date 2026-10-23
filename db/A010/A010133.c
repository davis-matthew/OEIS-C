/*
 * A010133  Continued fraction for sqrt(41).
 * https://oeis.org/A010133
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 8795, u16 n <= 8795, u32 n <= 8795, u64 n <= 8795,
 *            u128 n <= 8795, big any n
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
