/*
 * A227696  Expansion of f(x^3)^3 / f(x) in powers of x where f() is a
 *          Ramanujan theta function.
 * https://oeis.org/A227696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = value of multiplicative function b(3*n+1) given in OEIS
 *            A227696 by the rules: b(2^e) = -(1 + (-1)^e)/2 b(3^e) = 0 b(p^e)
 *            = e+1 if p ≡ 1 (mod 6) b(p^e) = 0 if p ≡ 5 (mod 6) and e odd
 *            b(p^e) = 1 if p ≡ 5 (mod 6) and e even
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/*  idx_t is an unsigned 64‑bit type,  seq_t is a signed type of the build.
    The function below complies with the required interface. */
seq_t a(idx_t n)
{
    /* Argument of the multiplicative function b. */
    uint64_t m = 3ULL * n + 1ULL;
    seq_t res = (seq_t)1;

    /* handle factor 2 */
    unsigned e2 = 0;
    while ((m & 1ULL) == 0ULL) {
        e2++;
        m >>= 1;          /* m /= 2 */
    }
    if (e2 > 0) {
        if (e2 & 1U)       /* e2 odd -> result zero */
            return (seq_t)0;
        /* e2 even -> multiply by -1 */
        res = -res;
    }

    /* handle factor 3 */
    if (m % 3ULL == 0) {
        /* any power of 3 gives 0 */
        return (seq_t)0;
    }
    /* remove factor 3 just in case for further loops */
    while (m % 3ULL == 0) {
        m /= 3ULL;
    }

    /* trial division for remaining primes */
    for (uint64_t p = 5ULL; p * p <= m; p += 2ULL) {
        if (m % p != 0ULL)
            continue;
        unsigned e = 0;
        while (m % p == 0ULL) {
            m /= p;
            e++;
        }
        if (p % 6ULL == 1ULL) {
            res *= (seq_t)(e + 1);
        } else { /* p % 6 == 5 */
            if (e & 1U)   /* odd exponent -> zero */
                return (seq_t)0;
            /* even exponent -> factor 1 (do nothing) */
        }
    }

    /* m may be a prime > sqrt(original m) */
    if (m > 1ULL) {
        /* m is prime; determine its class mod 6 */
        if (m % 6ULL == 1ULL) {
            res *= (seq_t)(1 + 1);     /* exponent is 1 -> (e+1)=2 */
        } else {                       /* m % 6 == 5 */
            /* exponent 1 is odd -> result zero */
            return (seq_t)0;
        }
    }

    return res;
}
