/*
 * A036162  a(n) = 6^n mod 229.
 * https://oeis.org/A036162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^n mod 229
 * Range:     u8 n <= 2000, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 1;               /* a(0) = 1 */
    seq_t base   = 6 % 229;         /* base for exponentiation */
    idx_t exp    = n;               /* exponent */

    while (exp)
    {
        if (exp & 1ULL)                 /* if the current bit is 1 */
        {
            /* multiply in wider type to avoid overflow on narrow seq_t */
            result = (seq_t)((uint32_t)result * base % 229);
        }

        exp >>= 1;                     /* move to next bit */

        /* square base for next bit, again using wider type */
        base = (seq_t)((uint32_t)base * base % 229);
    }

    return result;
}
