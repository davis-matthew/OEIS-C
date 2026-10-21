/*
 * A216127  a(n) = 6^n mod 1000.
 * https://oeis.org/A216127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^n mod 1000
 * Range:     u8 n <= 3, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    const uint64_t M = 1000ULL;
    uint64_t result = 1ULL;
    uint64_t base   = 6ULL % M;
    idx_t      exp   = n;

    while (exp > 0) {
        if (exp & 1ULL)
            result = (result * base) % M;
        exp >>= 1;
        base = (base * base) % M;
    }
    return (seq_t)result;
}
