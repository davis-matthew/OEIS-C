/*
 * A021623  Decimal expansion of 1/619.
 * https://oeis.org/A021623
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor((10 * (10^n mod 619)) / 619)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is the nth decimal digit of 1/619 (a(0) is the first digit). */
#include <stdint.h>

seq_t a(idx_t n) {
    /* Compute 10^n mod 619 using binary exponentiation */
    uint64_t mod  = 619ULL;
    uint64_t base = 10ULL;
    uint64_t pow   = 1ULL;
    idx_t     exp  = n;

    while (exp) {
        if (exp & 1ULL)
            pow = (pow * base) % mod;
        base = (base * base) % mod;
        exp >>= 1ULL;
    }

    /* The digit is floor((10 * pow) / 619) */
    uint64_t digit = (pow * 10ULL) / mod;
    return (seq_t)digit;
}
