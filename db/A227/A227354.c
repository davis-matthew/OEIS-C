/*
 * A227354  Expansion of 2 * a(q) - a(q^2) in powers of q where a() is a cubic
 *          AGM theta function.
 * https://oeis.org/A227354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 12 * b(n) (A227354) * b(n) is multiplicative with *
 *            b(2^e) = (1 + 3*(-1)^e) / 4 * b(3^e) = 1 * b(p^e) = e+1 if p ≡ 1
 *            (mod 6) * b(p^e) = (1 + (-1)^e) / 2 if p ≡ 5 (mod 6) * a(0) = 1
 *            is defined separately. * * For odd exponents of primes ≡ 5 (mod
 *            6) we obtain b(p^e) = 0, * hence a(n)=0. The...
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle the special offset value a(0) = 1. */
    if (n == 0)
        return (seq_t)1;

    /* Factor out powers of 2 first. */
    idx_t e2 = 0;
    idx_t t = n;
    while ((t & 1) == 0) {
        t >>= 1;
        ++e2;
    }

    seq_t N = 1;          /* numerator of b(n) (in integer form)   */
    seq_t D = 1;          /* denominator of b(n), initially 1        */

    if (e2 > 0) {
        /* b(2^e2) = (1 + 3*(-1)^e2)/4 */
        if ((e2 & 1) == 0) {          /* e2 even */
            N *= (seq_t)4;            /* val2 = 4   */
        } else {                      /* e2 odd  */
            N *= (seq_t)-2;           /* val2 = -2  */
        }
        D = (seq_t)4;                 /* denominator 4 if 2^e2 present */
    }

    /* Now factor the remaining odd part. */
    for (idx_t p = 3; p <= t / p; p += 2) {
        if (t % p == 0) {
            idx_t e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }
            if (p == 3) {
                /* b(3^e) = 1, no effect */
                continue;
            }
            if ((p % 6) == 1) {
                /* b(p^e) = e + 1 (integer) */
                N *= (seq_t)(e + 1);
            } else if ((p % 6) == 5) {
                if ((e & 1) != 0) {
                    /* odd exponent → factor 0, hence a(n) = 0 */
                    return (seq_t)0;
                }
                /* even exponent => factor is 1, ignore */
            }
        }
    }

    /* Any remaining prime factor t > 1 */
    if (t > 1) {
        /* t is a prime */
        idx_t e = 1;
        if (t == 3) {
            /* b(3) = 1, ignore */
        } else if ((t % 6) == 1) {
            N *= (seq_t)(e + 1);          /* e = 1 → factor 2 */
        } else if ((t % 6) == 5) {
            /* e = 1 odd → zero */
            return (seq_t)0;
        }
    }

    /* Compute a(n) = 12 * N / D.  The division is exact. */
    seq_t result = (seq_t)12 * N;
    if (D != 1)
        result = result / D;

    return result;
}
