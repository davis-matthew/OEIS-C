/*
 * A021249  Decimal expansion of 1/245.
 * https://oeis.org/A021249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1)/245) mod 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes the n‑th decimal digit of 1/245 (0‑based). */
seq_t a(idx_t n)
{
    /* Use 64‑bit arithmetic – modulus is very small */
    uint64_t exp = (uint64_t)n + 1;      /* exponent = n+1 */
    uint64_t base = 10 % 245;
    uint64_t res245  = 1 % 245;
    uint64_t res2450 = 1 % 2450;
    uint64_t b245  = base;
    uint64_t b2450 = base % 2450;
    /* modular exponentiation for modulus 245 */
    while (exp) {
        if (exp & 1)
            res245 = (res245 * b245) % 245;
        b245 = (b245 * b245) % 245;
        /* modular exponentiation for modulus 2450 */
        if (exp & 1)
            res2450 = (res2450 * b2450) % 2450;
        b2450 = (b2450 * b2450) % 2450;
        exp >>= 1;
    }
    /* Now compute digit: (res2450 - res245)/245 */
    uint64_t digit = (res2450 - res245) / 245;
    return (seq_t)digit;
}
