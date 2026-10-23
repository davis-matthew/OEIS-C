/*
 * A021798  Decimal expansion of 1/794.
 * https://oeis.org/A021798
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10 * (10^n mod 794) / 794), i.e. the n-th digit
 *            (0‑indexed) of the decimal expansion of 1/794
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint64_t MOD_794 = 794ULL;

/* Compute 10^exp mod 794. exp >= 0. */
static uint64_t pow10_mod(idx_t exp) {
    uint64_t result = 1ULL % MOD_794;
    uint64_t base   = 10ULL % MOD_794;
    while (exp) {
        if (exp & 1ULL)
            result = (result * base) % MOD_794;
        base = (base * base) % MOD_794;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    uint64_t rem   = pow10_mod(n);          /* 10^n mod 794, 0 <= rem < 794 */
    uint64_t digit = (rem * 10ULL) / MOD_794;  /* 0 .. 9 */
    return (seq_t)digit;
}
