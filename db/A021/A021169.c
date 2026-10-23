/*
 * A021169  Decimal expansion of 1/165.
 * https://oeis.org/A021169
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1)/165) mod 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth digit of the decimal expansion of 1/165 */
#include <stdint.h>

seq_t a(idx_t n)
{
    const uint64_t MOD = 1650ULL;          /* 165 * 10 */
    uint64_t exp   = n + 1ULL;             /* exponent for 10^(n+1) */
    uint64_t result = 1ULL % MOD;          /* current power modulo MOD */
    uint64_t base = 10ULL % MOD;           /* base 10 modulo MOD */

    while (exp > 0ULL) {
        if (exp & 1ULL)
            result = (result * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1ULL;
    }

    uint64_t digit = result / 165ULL;       /* digit: floor(result/165) */
    return (seq_t)digit;
}
