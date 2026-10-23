/*
 * A115607  Sum of odd divisors of n times (-1)^(n+1).
 * https://oeis.org/A115607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look, mult
 * Formula:   a(n) = -(-1)^n * (sum of odd divisors of n) (equivalently, a(n)
 *            = sigma(odd_part(n)) if n is odd, and a(n) = -sigma(odd_part(n))
 *            if n is even)
 * Range:     i8 n <= 12, i16 n <= 640, i32 n <= 8873, i64 n <= 8873,
 *            i128 n <= 8873, big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* odd part of n */
    uint64_t m = n;
    while ((m & 1ULL) == 0) m >>= 1;

    seq_t sigma = 1;      /* result of sigma(odd_part) */
    uint64_t t = m;       /* remaining factor to factor */

    for (uint64_t p = 3; p * p <= t; p += 2) {
        if (t % p == 0) {
            /* count exponent e of prime p */
            unsigned e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }
            /* term = (p^(e+1)-1)/(p-1) */
            seq_t term = (seq_pow((seq_t)p, e + 1) - 1) / (seq_t)(p - 1);
            sigma = sigma * term;
        }
    }
    if (t > 1) {   /* remaining prime factor with exponent 1 */
        seq_t term = (seq_pow((seq_t)t, 2) - 1) / (seq_t)(t - 1);
        sigma = sigma * term;
    }

    /* apply sign: if n is even, negate the result */
    if ((n & 1) == 0) {
        sigma = -sigma;
    }
    return sigma;
}
