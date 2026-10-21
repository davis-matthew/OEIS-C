/*
 * A086227  a(n) = Sum_{1<=k<=4*n, gcd(k,n)=1} (i^k*tan(k*Pi/(4*n)))/(4*i),
 *          where i is the imaginary unit.
 * https://oeis.org/A086227
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^(n+1) * h(-4 n^2) / 2 where h(D) is the class number
 *            of the quadratic order of discriminant D = -4 n^2. Using the
 *            conductor formula h(-4 n^2) = ∏_{p^e || n} p^{e-1} * (p - χ(p)),
 *            with χ(p) = 1 if p ≡ 1 (mod 4), χ(p) = -1 if p ≡ 3 (mod 4), and
 *            χ(2) = 0. This yields an integer class num...
 * Range:     i8 n <= 187, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* n ≥ 2 is assumed by the caller; for n < 2 return 0 (unused). */
    if (n < 2) return (seq_t)0;

    idx_t temp = n;
    seq_t h = 1;            /* class number accumulator */

    /* trial division to factor n */
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            /* exponent of p in n */
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }

            /* compute p^(e-1) */
            seq_t pow = 1;
            for (idx_t i = 0; i < e - 1; ++i)
                pow *= (seq_t)p;

            /* factor (p - χ(p)) */
            seq_t mul;
            if (p == 2) {                 /* χ(2) = 0  →  p - χ = p */
                mul = (seq_t)p;
            } else {
                if ((p & 3) == 1)          /* p ≡ 1 (mod 4)  →  χ = +1 */
                    mul = (seq_t)(p - 1);
                else                        /* p ≡ 3 (mod 4)  →  χ = -1 */
                    mul = (seq_t)(p + 1);
            }

            h *= pow * mul;
        }
    }

    /* if a prime factor > sqrt(n) remains */
    if (temp > 1) {
        idx_t p = temp;        /* exponent is 1, so e-1 = 0, pow = 1 */
        seq_t mul;
        if (p == 2) {
            mul = (seq_t)p;
        } else {
            if ((p & 3) == 1)
                mul = (seq_t)(p - 1);
            else
                mul = (seq_t)(p + 1);
        }
        h *= mul;              /* pow = 1, so multiply directly */
    }

    /* a(n) = (-1)^(n+1) * h / 2 */
    seq_t result = h / 2;
    if ((n & 1) == 0)          /* n even → multiply by -1 */
        result = -result;

    return result;
}
