/*
 * A014393  Final 2 digits of 9^n.
 * https://oeis.org/A014393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 9^n mod 100
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Fast modular exponentiation of 9^n modulo 100.
       All intermediate results fit in 16 bits, so we
       use an unsigned int for the arithmetic. */
    unsigned int result = 1;
    unsigned int base   = 9;
    idx_t exp = n;

    while (exp > 0) {
        if (exp & 1) {
            result = (result * base) % 100;
        }
        base = (base * base) % 100;
        exp >>= 1;
    }
    return (seq_t)result;
}
