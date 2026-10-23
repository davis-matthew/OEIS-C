/*
 * A199106  a(n) = 4*12^n - 1.
 * https://oeis.org/A199106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*12^n - 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* binary exponentiation to compute 12^n */
    seq_t pow12 = 1;   /* accumulated result */
    seq_t base  = 12;  /* current power of 12 (2^k) */
    idx_t exp = n;

    while (exp)
    {
        if (exp & 1)
            pow12 = pow12 * base;

        exp >>= 1;
        if (exp)           /* avoid extra multiply when exponent becomes zero */
            base  = base * base;
    }

    /* a(n) = 4 * 12^n - 1 */
    seq_t result = pow12 * 4;
    result = result - 1;
    return result;
}
