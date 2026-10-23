/*
 * A162397  a(n) = n * Kronecker(-3, n).
 * https://oeis.org/A162397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = n * Kronecker(-3, n)
 * Range:     i8 n <= 129, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes  a(n) = n * Kronecker(‑3, n)  for n >= 1.
   The Kronecker symbol is completely multiplicative;  it is:
   • 0 if 3 | n;
   • 1 if n ≡ 1 (mod 3);
   • –1 if n ≡ 2 (mod 3).
   The product n * symbol is achieved by factoring n and applying
   the sign rules for each prime power.
*/
seq_t a(idx_t n)
{
    if (n == 0)                  /* safety guard, offset is 1 */
        return 0;
    if (n % 3 == 0)              /* a(3k) = 0 */
        return 0;

    uint64_t m      = n;
    seq_t   result  = 1;
    int     sign    = 1;

    /* Handle factor 2 separately – 2 ≡ 2 (mod 3) */
    if ((m & 1) == 0) {
        uint64_t e = 0;
        while ((m & 1) == 0) {
            m >>= 1;
            ++e;
        }
        seq_t mult = 1;
        for (uint64_t i = 0; i < e; ++i)
            mult *= (seq_t)2;
        result *= mult;          /* absolute value contribution */
        if (e & 1)               /* odd exponent flips the sign */
            sign = -sign;
    }

    /* Trial division by odd primes up to √m */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            seq_t mult = 1;
            for (uint64_t i = 0; i < e; ++i)
                mult *= (seq_t)p;
            result *= mult;
            if ((p % 3) == 2 && (e & 1))   /* odd exponent of a prime ≡ 2 (mod 3) */
                sign = -sign;
        }
    }

    /* Remaining prime factor, if any */
    if (m > 1) {
        seq_t mult = (seq_t)m;
        result *= mult;
        if ((m % 3) == 2)               /* exponent is 1 */
            sign = -sign;
    }

    return (sign == 1) ? result : -result;
}
