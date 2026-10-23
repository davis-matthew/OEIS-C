/*
 * A021271  Decimal expansion of 1/267.
 * https://oeis.org/A021271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = nth digit of decimal expansion of 1/267 (base 10). The
 *            digit d_n is given by d_n = ⌊ (10^n mod 267) · 10 / 267 ⌋ We
 *            compute 10^n mod 267 efficiently using binary exponentiation
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline uint64_t pow10mod(idx_t exp)
{
    const uint64_t mod = 267ULL;
    uint64_t result = 1ULL;
    uint64_t base   = 10ULL % mod;
    while (exp > 0) {
        if (exp & 1ULL)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1ULL;
    }
    return result;
}

seq_t a(idx_t n)
{
    uint64_t rem   = pow10mod(n);      /* 10^n mod 267 */
    uint64_t digit = (rem * 10ULL) / 267ULL;
    return (seq_t)digit;
}
