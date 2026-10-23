/*
 * A011609  Legendre symbol (n,131).
 * https://oeis.org/A011609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = n^65 (mod 131)
 * Range:     i8 n <= 2, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    const seq_t mod = 131;          /* prime modulus */
    seq_t m   = n % mod;            /* reduce the index modulo 131 */
    seq_t res = 1;                   /* result of the modular exponentiation */
    seq_t base = m;
    idx_t exp = 65;                  /* exponent (p-1)/2 for p = 131 */

    /* binary exponentiation */
    while (exp != 0) {
        if (exp & 1)
            res = (res * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }

    /* map 0 -> 0, 1 -> 1, 130 -> -1 */
    if (res == 0)
        return 0;
    if (res == 1)
        return 1;
    return -1;
}
