/*
 * A040387  Continued fraction for sqrt(408).
 * https://oeis.org/A040387
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 20, a(n) = 5*(9+7*(-1)^n)/2 for n>0
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)20;           /* a(0) = 20 */

    /* For n>0: even n -> 40, odd n -> 5 */
    if ((n & 1U) != 0)
        return (seq_t)5;            /* odd n */
    else
        return (seq_t)40;           /* even n */
}
