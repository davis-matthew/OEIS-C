/*
 * A021025  Decimal expansion of 1/21.
 * https://oeis.org/A021025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = ((10^(n+1) / 21) mod 10)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The digit of the decimal expansion of 1/21 at position n (0‑based). */

#include <stdint.h>

/* Small modular exponentiation using 64‑bit integers. */
static seq_t powmod_uint(uint64_t base, idx_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp) {
        if (exp & 1) result = (result * b) % mod;
        b = (b * b) % mod;
        exp >>= 1;
    }
    return (seq_t)result;
}

seq_t a(idx_t n)
{
    /* Compute 10^(n+1) mod 21 and mod 210. */
    seq_t r = powmod_uint(10ULL, n + 1, 21ULL);   /* remainder modulo 21 */
    seq_t s = powmod_uint(10ULL, n + 1, 210ULL);  /* remainder modulo 210 */

    /* Digit = (s - r) / 21. s ≡ r (mod 21) and s < 210, so the result is 0–9. */
    return (s - r) / 21;
}
