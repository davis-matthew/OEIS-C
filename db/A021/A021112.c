/*
 * A021112  Decimal expansion of 1/108.
 * https://oeis.org/A021112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/108) mod 10
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is equivalent to a(n) = floor((10^(n+1) mod 1080)/108) */
#include <stdint.h>

seq_t a(idx_t n)
{
    const unsigned int MOD = 1080U;          /* 108*10 */
    const unsigned int DEN = 108U;           /* divisor for the final digit */

    seq_t result = 1 % MOD;   /* result of modular exponentiation */
    seq_t base   = 10 % MOD;  /* base for exponentiation (always 10) */
    idx_t exp = n + 1;        /* exponent (n+1) */

    /* binary exponentiation modulo MOD */
    while (exp)
    {
        if (exp & 1)
            result = (seq_t)(((seq_t)result * base) % MOD);
        base = (seq_t)(((seq_t)base * base) % MOD);
        exp >>= 1;
    }

    /* Now result = 10^(n+1) mod 1080 */
    seq_t s = result;
    return s / DEN;
}
