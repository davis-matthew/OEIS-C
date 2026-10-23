/*
 * A040703  Continued fraction for sqrt(731).
 * https://oeis.org/A040703
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

/* 
   Continued fraction terms for sqrt(731):
   a(0) = 27
   for n ≥ 1: a(n) = 27 if n is odd, 54 if n is even
*/

seq_t a(idx_t n)
{
    if (n == 0) {
        return 27;
    }
    /* odd n → 27, even n → 54 */
    return ((n & 1U) ? 27 : 54);
}
