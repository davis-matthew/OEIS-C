/*
 * A021649  Decimal expansion of 1/645.
 * https://oeis.org/A021649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/645) mod 10 Implementation uses modular
 *            exponentiation of 10 modulo 6450. The decimal expansion of 1/645
 *            repeats after the first two leading zeros, so this formula
 *            captures all digits for n >= 0
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    const uint64_t MOD = 6450ULL;       /* 645 * 10 */
    uint64_t exp   = n + 1;             /* exponent n+1 */
    uint64_t base  = 10 % MOD;
    uint64_t result = 1 % MOD;

    while (exp) {
        if (exp & 1ULL)
            result = (result * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1;
    }
    return (seq_t)(result / 645ULL);
}
