/*
 * A052782  a(n) = (5*n+1)^(n-1).
 * https://oeis.org/A052782
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n+1)^(n-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(0) is defined as 1 by convention */
    if (n == 0)
        return 1;

    /* base = 5*n + 1 */
    seq_t base = (seq_t)n * 5 + 1;
    idx_t exp = n - 1;

    /* fast exponentiation by squaring */
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}
