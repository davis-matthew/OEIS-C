/*
 * A021425  Decimal expansion of 1/421.
 * https://oeis.org/A021425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/421) mod 10 * = ((10^(n+1) mod 4210) /
 *            421)
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Validated: partial, pass, 99 terms through n = 98 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t powmod_10(idx_t exp) {
    /* Compute 10^exp modulo 4210 using binary exponentiation. */
    const uint64_t mod = 4210u;
    uint64_t result = 1u % mod;
    uint64_t base   = 10u % mod;
    while (exp) {
        if (exp & 1u)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1u;
    }
    return (seq_t)result;   /* value < 4210 */
}

seq_t a(idx_t n) {
    /* Digit number n of the decimal expansion of 1/421. */
    idx_t exp = n + 1u;          /* compute 10^(n+1) */
    seq_t r   = powmod_10(exp);  /* r = 10^(n+1) mod 4210   */
    return (seq_t)(r / 421u);    /* floor(r/421) gives the digit */
}
