/*
 * A013729  a(n) = 24^(2*n + 1).
 * https://oeis.org/A013729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 24^(2*n + 1)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 6, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case n == 0 */
    if (n == 0u) {
        return (seq_t)24;
    }

    /* Calculate 576^n using exponentiation by squaring */
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)576;
    idx_t exp    = n;

    while (exp > 0u) {
        if (exp & 1u) {
            result = result * base;
        }
        base = base * base;
        exp >>= 1u;
    }

    /* Multiply by 24 to obtain 24 * 576^n = 24^(2*n+1) */
    return (seq_t)24 * result;
}
