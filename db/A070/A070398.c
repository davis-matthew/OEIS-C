/*
 * A070398  a(n) = 6^n mod 29.
 * https://oeis.org/A070398
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6^n mod 29 The computation uses binary exponentiation in
 *            O(log n) time, with no table lookup or recursion
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* modulus fits in uint64_t; 6^n mod 29 will also fit. */
    const uint64_t MOD = 29;
    uint64_t base = 6;
    uint64_t result = 1;
    /* binary exponentiation */
    while (n > 0) {
        if (n & 1ULL)
            result = (result * base) % MOD;
        base = (base * base) % MOD;
        n >>= 1ULL;
    }
    return (seq_t)result;
}
