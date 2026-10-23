/*
 * A021769  Decimal expansion of 1/765.
 * https://oeis.org/A021769
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = ((10^n mod 765) * 10) / 765
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This computes the n‑th decimal digit of 1/765 (a(0) is the first digit). */

#include <stdint.h>

static inline uint64_t modpow10(uint64_t exp)
{
    uint64_t base = 10;
    uint64_t result = 1;
    const uint64_t mod = 765;

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
    /* Compute 10^n mod 765 using fast exponentiation. */
    uint64_t r = modpow10((uint64_t)n);

    /* Compute the digit: floor((r * 10) / 765). */
    uint64_t digit = (r * 10) / 765;

    return (seq_t)digit;
}
