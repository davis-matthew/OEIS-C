/*
 * A021685  Decimal expansion of 1/681.
 * https://oeis.org/A021685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1) / 681) mod 10 which equals ((10^(n+1) mod
 *            6810) / 681)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* modulus 6810 = 10 * 681, divisor 681 */
    const uint32_t MOD  = 6810;
    const uint32_t DIV  = 681;

    /* fast exponentiation by squaring on 10^(n+1) mod 6810 */
    uint64_t exp   = n + 1;      /* exponent as 64‑bit unsigned */
    uint64_t base  = 10;
    uint64_t res   = 1 % MOD;    /* result of modular power */

    while (exp > 0)
    {
        if (exp & 1ULL)
            res = (res * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1ULL;
    }

    /* The desired decimal digit is the integer part of the quotient.
       Since MOD = 10 * DIV, res = 681 * (digit) + remainder,
       with 0 <= remainder < 681, so digit = res / 681. */
    return (seq_t)(res / DIV);
}
