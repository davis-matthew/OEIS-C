/*
 * A134756  Coefficients of a q-series of Zagier related to the Dedekind eta
 *          function.
 * https://oeis.org/A134756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = b(24*n + 1) where b() is multiplicative: * b(2^e) =
 *            b(3^e) = 0^e * b(p^e) = (1 + (-1)^e)/2 * p^(e/2) if p ≡ 1,11
 *            (mod 12) * b(p^e) = (1 + (-1)^e)/2 * (-p)^(e/2) if p ≡ 5,7 (mod
 *            12)
 * Range:     i8 n <= 714, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute m = 24*n + 1 (always odd) */
    idx_t m = 24 * n + 1;
    if (m == 1)                 /* b(1) = 1 */
        return (seq_t)1;

    seq_t result = 1;
    idx_t temp   = m;

    /* Trial factorisation up to sqrt(temp) */
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            /* Count exponent of prime p */
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }

            /* Zero contribution from 2 or 3 */
            if (p == 2 || p == 3)
                return (seq_t)0;

            /* Odd exponent forces result to 0 */
            if (e & 1)
                return (seq_t)0;

            idx_t k = e >> 1;          /* e/2 */
            seq_t pow = 1;
            for (idx_t i = 0; i < k; ++i)
                pow *= (seq_t)p;        /* p^k */

            /* Apply sign according to residue modulo 12 */
            if (p % 12 == 5 || p % 12 == 7) {
                if (k & 1)                     /* (-p)^k negative if k odd */
                    result *= -(pow);
                else
                    result *= pow;
            } else {                          /* p mod 1 or 11 -> positive */
                result *= pow;
            }
        }
    }

    /* If a prime factor > sqrt(m) remains, its exponent is 1 (odd) */
    if (temp > 1)
        return (seq_t)0;

    return result;
}
