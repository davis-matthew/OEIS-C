/*
 * A013805  a(n) = 16^(4*n + 3).
 * https://oeis.org/A013805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16^(4*n + 3)
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 3, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute exponent: e = 4*n + 3 (idx_t is unsigned 64-bit) */
    idx_t exp = 4 * n + 3;

    /* Base 16 fits in seq_t */
    seq_t result = 1;
    seq_t base   = 16;

    /* Binary exponentiation */
    while (exp)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}
