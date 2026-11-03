/*
 * A021773  Decimal expansion of 1/769.
 * https://oeis.org/A021773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1)/769) mod 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Validated: partial, pass, 99 terms through n = 98 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute 10^exp mod 7690 (769 * 10) using fast exponentiation. */
static inline uint64_t modpow10_exp(idx_t exp)
{
    const uint64_t MOD = 7690ULL;          /* 769 * 10 */
    uint64_t result = 1ULL;
    uint64_t base   = 10ULL % MOD;

    while (exp)
    {
        if (exp & 1ULL)
            result = (result * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1ULL;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* 10^(n+1) mod 7690 gives a remainder t such that
       floor(10^(n+1)/769) mod 10 == t / 769 (integer division). */
    uint64_t t = modpow10_exp(n + 1ULL);
    seq_t digit = (seq_t)(t / 769ULL);
    return digit;
}
