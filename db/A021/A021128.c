/*
 * A021128  Decimal expansion of 1/124.
 * https://oeis.org/A021128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1) / 124) mod 10 – the nth digit of the
 *            decimal expansion of 1/124
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use the identity: a(n) = floor( (10^(n+1) mod 1240) / 124 )   (since 10^(n+1) = 1240·q + r, r/124 = integer part modulo 10). */
    const uint64_t MOD = 1240ULL;   /* 1240 = 124 × 10 */
    uint64_t base = 10ULL % MOD;    /* base for modular exponentiation */
    uint64_t result = 1ULL;         /* accumulates 10^(n+1) mod 1240 */

    idx_t exp = n + 1;              /* exponent (may overflow only if n is at bit‑limit, which is not in usage). */

    /* fast modular exponentiation: compute 10^(n+1) mod 1240 */
    while (exp)
    {
        if (exp & 1ULL)
            result = (result * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1;
    }

    /* The digit is the integer part of result/124 (0–9). */
    return (seq_t)(result / 124ULL);
}
