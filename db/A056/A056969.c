/*
 * A056969  a(n) = 10^n modulo n.
 * https://oeis.org/A056969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10^n mod n
 * Range:     u8 n <= 271, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle n = 0 (should not occur) */
    if (n == 0)
        return (seq_t)0;

    idx_t result = 1 % n;
    idx_t base   = 10 % n;
    idx_t exp    = n;

    while (exp) {
        if (exp & 1)
            result = (result * base) % n;
        base = (base * base) % n;
        exp >>= 1;
    }

    return (seq_t)result;
}
