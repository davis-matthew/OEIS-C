/*
 * A070391  a(n) = 5^n mod 46.
 * https://oeis.org/A070391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5^n) mod 46
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute 5^n modulo 46 using exponentiation by squaring */
    const uint64_t mod = 46ULL;
    uint64_t result = 1;
    uint64_t base   = 5ULL;
    idx_t e = n;

    while (e) {
        if (e & 1ULL)
            result = (result * base) % mod;
        base = (base * base) % mod;
        e >>= 1;
    }

    return (seq_t)result;
}
