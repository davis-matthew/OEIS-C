/*
 * A021781  Decimal expansion of 1/777.
 * https://oeis.org/A021781
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = digit in the decimal expansion of 1/777 at position n *
 *            (zero‑based). It is computed as ((10^n mod 777)*10)/777
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    const uint64_t MOD = 777ULL;
    uint64_t exp  = (uint64_t)n;
    uint64_t base = 10ULL % MOD;
    uint64_t res  = 1ULL;

    while (exp) {
        if (exp & 1ULL)
            res = (res * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1ULL;
    }

    uint64_t digit = (res * 10ULL) / MOD;
    return (seq_t)digit;
}
