/*
 * A070409  a(n) = 7^n mod 23.
 * https://oeis.org/A070409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^n mod 23
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Modular exponentiation with small constant modulus */
    const uint64_t MOD = 23;
    uint64_t r = 1;
    uint64_t base = 7;
    uint64_t exp = n;

    while (exp) {
        if (exp & 1ULL) {
            r = (r * base) % MOD;
        }
        base = (base * base) % MOD;
        exp >>= 1ULL;
    }

    return (seq_t)r;
}
