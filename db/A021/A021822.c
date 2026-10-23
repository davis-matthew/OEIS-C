/*
 * A021822  Decimal expansion of 1/818.
 * https://oeis.org/A021822
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1)/818) mod 10 * = ((10^(n+1) mod 8180) /
 *            818) * This generates the nth decimal digit of 1/818
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* 64‑bit modular exponentiation (fast pow) */
static uint64_t modpow_uint64(uint64_t base, idx_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp > 0) {
        if (exp & 1ULL)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1ULL;
    }
    return result;
}

/* Sequence generator */
seq_t a(idx_t n)
{
    /* 818 * 10 = 8180 */
    const uint64_t MOD = 8180ULL;
    const uint64_t DIV = 818ULL;

    /* Compute 10^(n+1) mod 8180 */
    uint64_t r = modpow_uint64(10ULL, n + 1, MOD);

    /* Extract the digit */
    seq_t digit = (seq_t)(r / DIV);
    return digit;
}
