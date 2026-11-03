/*
 * A021068  Decimal expansion of 1/64.
 * https://oeis.org/A021068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10 * (10^n mod 64) / 64) (decimal digit of 1/64)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Validated: partial, pass, 99 terms through n = 98 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t modpow10u64(idx_t exp)
{
    /* Compute 10^exp mod 64 using binary exponentiation */
    const idx_t MOD = 64;
    uint64_t result = 1;
    uint64_t base   = 10;
    idx_t e = exp;
    while (e > 0)
    {
        if (e & 1ULL)
            result = (result * base) % MOD;
        base = (base * base) % MOD;
        e >>= 1;
    }
    return (seq_t)result;
}

seq_t a(idx_t n)
{
    /* digit at position n after the decimal point of 1/64 */
    seq_t r = modpow10u64(n);          /* r = 10^n mod 64, 0 <= r < 64 */
    seq_t d = (10 * r) / 64;           /* d = floor(10*r/64), last digit */
    return d;
}
