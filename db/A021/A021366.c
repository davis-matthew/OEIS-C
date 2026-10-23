/*
 * A021366  Decimal expansion of 1/362.
 * https://oeis.org/A021366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor((10^(n+1) / 362)) mod 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The nth decimal digit after the point in 1/362. */

#include <stdint.h>

static uint64_t powmod_uint64(uint64_t base, idx_t exp, uint64_t mod) {
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1U)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    const uint64_t denom = 362ULL;
    /* Compute r = 10^n mod 362 */
    uint64_t r = powmod_uint64(10ULL, n, denom);
    /* Digit = floor((r * 10) / 362) */
    return (seq_t)((r * 10ULL) / denom);
}
