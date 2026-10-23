/*
 * A021280  Decimal expansion of 1/276.
 * https://oeis.org/A021280
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10 * (10^n mod 276) / 276)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    const uint64_t MOD = 276;           /* 1/276 denominator */
    uint64_t exp = (uint64_t)n;         /* exponent */
    uint64_t base = 10 % MOD;
    uint64_t res = 1 % MOD;

    /* fast modular exponentiation: res = 10^n mod 276 */
    while (exp) {
        if (exp & 1ULL) {
            res = (res * base) % MOD;
        }
        base = (base * base) % MOD;
        exp >>= 1ULL;
    }

    /* digit = floor(10 * res / 276) */
    uint64_t digit = (10ULL * res) / MOD;
    return (seq_t)digit;
}
