/*
 * A021837  Decimal expansion of 1/833.
 * https://oeis.org/A021837
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1)/833) mod 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 8330 = 833 * 10, the modulus for the reduced exponentiation */
    const uint64_t MOD = 8330ULL;
    uint64_t base = 10ULL % MOD;
    uint64_t res  = 1ULL;
    idx_t exp = n + 1U;          /* exponent is n+1 */

    /* fast modular exponentiation: compute 10^(n+1) mod 8330 */
    while (exp > 0) {
        if (exp & 1ULL) {
            res = (res * base) % MOD;
        }
        base = (base * base) % MOD;
        exp >>= 1ULL;
    }

    /* the desired digit is floor(res/833) */
    return (seq_t)(res / 833ULL);
}
