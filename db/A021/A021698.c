/*
 * A021698  Decimal expansion of 1/694.
 * https://oeis.org/A021698
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10 * (10^n mod 694) / 694)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute rem = 10^n mod 694 */
    const uint64_t MOD = 694;
    uint64_t rem = 1;
    uint64_t base = 10 % MOD;
    idx_t exp = n;

    while (exp > 0) {
        if (exp & 1ULL)
            rem = (rem * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1;
    }

    /* digit = floor(rem*10 / 694) */
    uint64_t digit = (rem * 10ULL) / MOD;
    return (seq_t)digit;
}
