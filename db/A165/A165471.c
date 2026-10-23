/*
 * A165471  Legendre symbol (n,65537).
 * https://oeis.org/A165471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = Legendre symbol (n,65537) = n^32768 (mod 65537). Returns
 *            0 if n ≡ 0 (mod 65537), 1 if n is a quadratic residue, -1 if n
 *            is a non‑residue
 * Range:     i8 n <= 8635, i16 n <= 8635, i32 n <= 8635, i64 n <= 8635,
 *            i128 n <= 8635, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* modulus and exponent for Euler's criterion */
    const uint64_t P   = 65537UL;
    const uint64_t EXP = 32768UL;

    /* base = n mod P (fits in 16 bits) */
    uint64_t base = n % P;
    uint64_t res  = 1;

    /* binary exponentiation */
    uint64_t exp = EXP;
    while (exp) {
        if (exp & 1ULL)
            res = (res * base) % P;
        base = (base * base) % P;
        exp >>= 1ULL;
    }

    /* map the modular result to -1, 0, or 1 */
    if (res == 0)
        return (seq_t)0;
    else if (res == P - 1)   /* P-1 ≡ -1 (mod P) */
        return (seq_t)-1;
    else
        return (seq_t)1;
}
