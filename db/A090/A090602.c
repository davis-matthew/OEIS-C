/*
 * A090602  Number of n-element labeled groupoids with an identity.
 * https://oeis.org/A090602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^((n-1)^2 + 1)
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (For n = 0, a(0) = 0 by definition) */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* a(0) = 0 */

    /* Base of the power */
    seq_t base = (seq_t)n;
    /* Exponent = (n-1)^2 + 1 */
    idx_t t   = n - 1;
    idx_t exp = t * t + 1;

    /* Fast binary exponentiation */
    seq_t result = 1;
    while (exp != 0)
    {
        if (exp & 1) result = result * base;
        exp >>= 1;
        if (exp) base = base * base;     /* avoid one unnecessary square on last iteration */
    }
    return result;
}
