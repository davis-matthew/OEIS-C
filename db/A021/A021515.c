/*
 * A021515  Decimal expansion of 1/511.
 * https://oeis.org/A021515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1) / 511) mod 10
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The digit at position n in the decimal expansion of 1/511. */
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* Compute 10^(n+1) modulo 5110.
       Since (10^(n+1)) div 511 mod 10 depends only on
       10^(n+1) mod 5110, we can take the modulus early.    */
    const seq_t MODULO   = 5110;      /* 511 * 10 */
    const seq_t DIVISOR   = 511;      /* divisor of the original fraction */
    const seq_t TEN       = 10;

    /* exponent = n + 1 (wraps around on UINT64_MAX, but this case is irrelevant) */
    idx_t exp = n + 1;

    /* a(n) = floor(10^(n+1) / 511) % 10
       = (10^(n+1) mod 5110) / 511            */
    seq_t rem = mod_pow(TEN, exp, MODULO);
    seq_t digit = rem / DIVISOR;   /* rem < 5110, so digit ∈ [0,9] */
    return digit;
}
