/*
 * A013750  a(n) = 12^(3*n + 1).
 * https://oeis.org/A013750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12^(3*n + 1)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* exponent = 3*n + 1  (fits in idx_t for all reasonable n) */
    idx_t exp = n * 3 + 1;

    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)12;

    /* fast exponentiation by squaring */
    while (exp > 0)
    {
        if (exp & 1) result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}
