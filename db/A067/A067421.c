/*
 * A067421  Sixth column of triangle A067417.
 * https://oeis.org/A067421
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*(18)^(n-1) for n >= 1, a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)8;

    /* compute 18^(n-1) using binary exponentiation */
    idx_t exp = n - 1;
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)18;

    while (exp > 0)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }

    return (seq_t)8 * result;
}
