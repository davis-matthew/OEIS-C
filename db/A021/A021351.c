/*
 * A021351  Decimal expansion of 1/347.
 * https://oeis.org/A021351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = ⌊10^(n+1)/347⌋ mod 10
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute 10^(n+1) mod 3470, then divide by 347 to get the
       digit (0–9). */
    const uint64_t MOD = 3470ULL;            /* 347 * 10 */
    const uint64_t DIV = 347ULL;
    __uint128_t res = 1;
    __uint128_t base = 10 % MOD;
    idx_t e = n + 1;
    while (e) {
        if (e & 1)          /* res = res * base mod MOD */
            res = (res * base) % MOD;
        base = (base * base) % MOD;
        e >>= 1;
    }
    uint64_t val = (uint64_t)res;            /* 0 .. 3469 */
    return (seq_t)(val / DIV);               /* digit 0..9 */
}
