/*
 * A021845  Decimal expansion of 1/841.
 * https://oeis.org/A021845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor((10^n mod 841) * 10 / 841)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   where a(0) is the first digit after the decimal point of 1/841 */
#include <stdint.h>

seq_t a(idx_t n) {
    /* Compute 10^n modulo 841 using binary exponentiation.
       All intermediate values are < 841*841 < 10^6, so a 64‑bit
       unsigned integer is sufficient and independent of the build
       type of seq_t. */
    const uint64_t MOD = 841ULL;
    uint64_t r = 1ULL;          /* r = 10^0 mod MOD */
    uint64_t base = 10ULL % MOD;
    idx_t e = n;                /* working copy of the exponent */

    while (e) {
        if (e & 1ULL)          /* if the current bit is set */
            r = (r * base) % MOD;
        base = (base * base) % MOD;
        e >>= 1ULL;
    }

    /* The decimal digit a(n) is the integer part of
       (r * 10) / MOD.  r < MOD, so the product < 8410, tiny. */
    uint64_t digit = (r * 10ULL) / MOD;
    return (seq_t)digit;
}
