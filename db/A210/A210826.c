/*
 * A210826  G.f.: Sum_{n>=1} a(n)*x^n/(1 - x^n) = Sum_{n>=1} x^(n^3).
 * https://oeis.org/A210826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) is the multiplicative function defined by a(p^e) = -1 +
 *            ((e+2) mod 3) for every prime p and exponent e ≥ 1. The base
 *            value is a(1) = 1. Thus a(p)=−1, a(p²)=0, a(p³)=1, and the value
 *            on a composite number is the product of its prime‑power values
 * Range:     i8 n <= 1035, i16 n <= 1035, i32 n <= 1035, i64 n <= 1035,
 *            i128 n <= 1035, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Offset starts at 1; guard against a 0 argument if it ever appears. */
    if (n == 0)
        return 0;

    if (n == 1)
        return 1;

    seq_t result = 1;
    idx_t tmp = n;

    /* Handle factor 2 separately. */
    if ((tmp & 1U) == 0) {
        unsigned e = 0;
        while ((tmp & 1U) == 0) {
            tmp >>= 1;
            ++e;
        }
        seq_t term = (seq_t)((e + 2) % 3) - 1;   /* –1, 0 or 1 */
        if (term == 0)
            return 0;
        result *= term;
    }

    /* Trial division by odd primes up to sqrt(tmp). */
    for (idx_t p = 3; p <= tmp / p; p += 2) {
        if (tmp % p == 0) {
            unsigned e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            seq_t term = (seq_t)((e + 2) % 3) - 1;
            if (term == 0)
                return 0;
            result *= term;
        }
    }

    /* If something larger than 1 remains, it is a prime factor with exponent 1. */
    if (tmp > 1) {
        seq_t term = (seq_t)((1 + 2) % 3) - 1;   /* equals –1 */
        result *= term;
    }

    return result;
}
