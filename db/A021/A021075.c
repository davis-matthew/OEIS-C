/*
 * A021075  Decimal expansion of 1/71.
 * https://oeis.org/A021075
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor((10 * (10^n mod 71)) / 71)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Validated: partial, pass, 105 terms through n = 104 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   The n-th digit of the decimal expansion of 1/71. */

#include <stdint.h>

/* Compute 10^n mod 71 using binary exponentiation. */
static inline uint64_t mod10_pow_n(idx_t n)
{
    uint64_t result = 1ULL;
    uint64_t base   = 10ULL;
    const uint64_t mod = 71ULL;

    while (n) {
        if (n & 1ULL)
            result = (result * base) % mod;
        base = (base * base) % mod;
        n >>= 1ULL;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* 10^n mod 71 */
    uint64_t r = mod10_pow_n(n);
    /* digit = floor((10 * r) / 71) */
    uint64_t digit = (r * 10ULL) / 71ULL;
    return (seq_t)digit;
}
