/*
 * A199111  a(n) = 8*3^n + 1.
 * https://oeis.org/A199111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*3^n + 1
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 38, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the value of A199111 for any n >= 0. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute 3^n with binary exponentiation */
    seq_t result = 1;
    seq_t base = 3;
    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1ULL)
            result = result * base;
        base = base * base;
        exp >>= 1;
    }
    /* a(n) = 8*3^n + 1 */
    return result * 8 + 1;
}
