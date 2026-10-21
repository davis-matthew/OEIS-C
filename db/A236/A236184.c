/*
 * A236184  Decimal expansion of 1/65537.
 * https://oeis.org/A236184
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/65537) mod 10
 * Range:     u8 n <= 8820, u16 n <= 8820, u32 n <= 8820, u64 n <= 8820,
 *            u128 n <= 8820, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    const uint64_t MOD = 655370ULL;      /* 65537 * 10 */
    uint64_t exp = n + 1ULL;             /* exponent n+1 */
    uint64_t base = 10ULL % MOD;         /* base 10 modulo MOD */
    uint64_t result = 1ULL % MOD;        /* running product modulo MOD */

    while (exp) {
        if (exp & 1ULL)
            result = (result * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1ULL;
    }

    return (seq_t)(result / 65537ULL);   /* digit is the integer part of result/65537 */
}
