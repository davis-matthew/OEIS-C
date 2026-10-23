/*
 * A040860  Continued fraction for sqrt(890).
 * https://oeis.org/A040860
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(n) = 29 for n=0; * a(n) = 1 if n is odd; * a(n) = 4 if n has
 *            exactly one factor 2; * a(n) = 58 otherwise
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the offset special case */
    if (n == 0)
        return (seq_t)29;

    /* Odd n → 1 */
    if ((n & 1ULL) == 1)
        return (seq_t)1;

    /* n is even: count the exponent of 2 */
    idx_t t = n >> 1;          /* remove one factor of 2 */
    if ((t & 1ULL) == 1)        /* only one factor of 2 */
        return (seq_t)4;
    else                        /* exponent >= 2 */
        return (seq_t)58;
}
