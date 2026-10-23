/*
 * A013740  a(n) = 7^(3*n + 1).
 * https://oeis.org/A013740
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^(3*n + 1)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* exponent = 3*n + 1 */
    idx_t e = 3 * n + 1;

    /* fast exponentiation: result = base^e, base = 7 */
    seq_t result = 1;
    seq_t base   = 7;

    while (e)
    {
        if (e & 1) result *= base;
        base *= base;
        e >>= 1;
    }

    return result;
}
