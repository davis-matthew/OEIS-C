/*
 * A162766  a(n) = 3*a(n-2) for n > 2; a(1) = 4, a(2) = 3.
 * https://oeis.org/A162766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-2); a(1) = 4, a(2) = 3
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 38, u64 n <= 80, u128 n <= 160,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for fixed-width integer types if needed */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)3;

    seq_t result = 1;
    idx_t exp = n / 2;            /* floor(n/2) */
    for (idx_t i = 0; i < exp; ++i)
        result *= (seq_t)3;       /* multiply by 3, exp times */

    if (n & 1)                    /* if n is odd, multiply by 4 */
        result *= (seq_t)4;

    return result;
}
