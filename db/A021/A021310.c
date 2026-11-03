/*
 * A021310  Decimal expansion of 1/306.
 * https://oeis.org/A021310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1)/306) mod 10 The n-th decimal digit of
 *            1/306
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Validated: partial, pass, 99 terms through n = 98 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute 10^exp (mod mod) for arbitrary-precision seq_t */
static seq_t power10_mod(idx_t exp, seq_t mod)
{
    seq_t base = 10 % mod;
    seq_t result = 1 % mod;
    idx_t e = exp;
    while (e > 0) {
        if (e & 1ULL)
            result = (result * base) % mod;
        base = (base * base) % mod;
        e >>= 1ULL;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* 3060 = 306 * 10 is the period of the residues needed for the
       n-th digit (see explanation in the comment above). */
    const seq_t MOD = 3060;
    /* Compute 10^(n+1) modulo 3060 */
    seq_t pow_mod = power10_mod(n + 1, MOD);
    /* The desired digit is floor( pow_mod / 306 ) */
    return pow_mod / 306;
}
