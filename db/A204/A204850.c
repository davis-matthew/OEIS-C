/*
 * A204850  Expansion of f(x)^3 - 9 * x * f(x^9)^3 in powers of x where f() is
 *          a Ramanujan theta function.
 * https://oeis.org/A204850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = b(8*n+1) where b() is multiplicative with * b(2^e) = 0^e,
 *            * b(3^e) = - (1 + (-1)^e) * 3^(e/2), * b(p^e) = (1 + (-1)^e)/2 *
 *            p^(e/2) if p ≡ 1,3 (mod 8), * b(p^e) = (1 + (-1)^e)/2 *
 *            (-p)^(e/2) if p ≡ 5,7 (mod 8)
 * Range:     i8 n <= 594, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* factor m = 8*n + 1 */
    idx_t m = 8 * n + 1;
    seq_t result = (seq_t)1;
    idx_t t = m;

    /* handle p = 3 specially */
    if (t % 3 == 0) {
        idx_t e = 0;
        while (t % 3 == 0) { t /= 3; ++e; }
        if (e & 1) return (seq_t)0;          /* odd exponent */
        idx_t k = e >> 1;                    /* exponent/2 */
        /* b(3^e) = -2 * 3^k */
        result *= (seq_t)(-2);
        result *= big_pow((seq_t)3, (seq_t)k);
    }

    /* trial division by odd primes >= 5 */
    for (idx_t p = 5; p * p <= t; p += 2) {
        if (t % p == 0) {
            idx_t e = 0;
            while (t % p == 0) { t /= p; ++e; }
            if (e & 1) return (seq_t)0;      /* odd exponent */
            idx_t k = e >> 1;
            seq_t term = big_pow((seq_t)p, (seq_t)k);

            /* sign adjustment for primes ≡ 5 or 7 (mod 8) */
            if ((p & 7) == 5 || (p & 7) == 7) {
                if (k & 1) result *= (seq_t)(-1);
            }

            result *= term;
        }
    }

    /* any remaining prime factor must have exponent 1, hence odd */
    if (t > 1) return (seq_t)0;

    return result;
}
