/*
 * A021406  Decimal expansion of 1/402.
 * https://oeis.org/A021406
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor((10^(n+1) / 402)) mod 10. We compute 10^(n+1)
 *            modulo 4020, then divide by 402
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline uint64_t powmod_uint64(uint64_t base, idx_t exp, uint64_t mod)
{
    uint64_t result = 1;
    while (exp)
    {
        if (exp & 1ULL)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Compute 10^(n+1) mod 4020, then take floor(.../402). */
    const uint64_t MOD  = 4020ULL;
    const uint64_t BASE = 10ULL % MOD;
    uint64_t rem   = powmod_uint64(BASE, n + 1, MOD);
    uint64_t digit = rem / 402ULL;
    return (seq_t)digit;
}
