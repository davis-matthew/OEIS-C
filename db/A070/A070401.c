/*
 * A070401  a(n) = 6^n mod 47.
 * https://oeis.org/A070401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^n mod 47
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t, if needed */

seq_t a(idx_t n)
{
    /* Compute 6^n modulo 47 using binary exponentiation. */
    seq_t result = 1;
    seq_t base   = 6;
    const seq_t mod = 47;

    while (n > 0) {
        if (n & 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        n >>= 1;
    }
    return result;
}
