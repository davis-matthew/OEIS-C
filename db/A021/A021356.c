/*
 * A021356  Decimal expansion of 1/352.
 * https://oeis.org/A021356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor((10^n mod 352) * 10 / 352)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This yields the nth decimal digit (0‑indexed) of 1/352 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute r = 10^n mod 352 using fast exponentiation */
    uint64_t r = 1;          /* r will hold 10^k mod 352 */
    uint64_t base = 10;      /* base = 10^1 mod 352 */
    const uint64_t mod  = 352;
    idx_t e = n;             /* exponent */

    while (e > 0) {
        if (e & 1ULL)
            r = (r * base) % mod;
        base = (base * base) % mod;
        e >>= 1;
    }

    /* a(n) = floor(r * 10 / 352) */
    uint64_t digit = (r * 10) / mod;
    return (seq_t)digit;
}
