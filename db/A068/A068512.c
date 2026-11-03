/*
 * A068512  Numerators of arithmetic derivative of n/2: A003415(n)/2 - n/4;
 *          denominators: A010685.
 * https://oeis.org/A068512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (A003415(n)/2 - n/4) reduced, returning the numerators of
 *            the fraction A003415(n)/2 - n/4. The implementation uses the
 *            arithmetic derivative function arithmetic_derivative(n) and
 *            simplifies the resulting rational with denominator 4 by dividing
 *            out the gcd with 4
 * Range:     i8 n <= 31, i16 n <= 61, i32 n <= 61, i64 n <= 61, i128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/gcd.h"

/* arithmetic derivative: d(1)=0, d(p)=1 for prime p,
   d(mn)=m*d(n)+n*d(m). For a prime factorization n=∏p^e,
   d(n) = n * Σ (e/p).                                                         */
static inline seq_t arithmetic_derivative(uint64_t m)
{
    seq_t result = 0;
    uint64_t orig = m;
    if (m == 0) return 0;          /* won't be used, but safe */

    /* handle factor 2 */
    uint64_t exp2 = 0;
    while ((m & 1) == 0) {
        m >>= 1;
        exp2++;
    }
    if (exp2) {
        result += (seq_t)exp2 * (seq_t)(orig >> 1);   /* orig/2 */
    }

    /* odd prime factors */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            uint64_t exp = 0;
            while (m % p == 0) {
                m /= p;
                exp++;
            }
            result += (seq_t)exp * (seq_t)(orig / p);
        }
    }

    /* remaining prime */
    if (m > 1) {
        result += (seq_t)(orig / m);   /* exp = 1 */
    }

    return result;
}

seq_t a(idx_t n)
{
    /* Compute the arithmetic derivative d(n). */
    seq_t d = arithmetic_derivative((uint64_t)n);   /* d is signed */

    /* numerator = 2*d - n, denom = 4. */
    seq_t numer = (d << 1) - (seq_t)n;
    seq_t denom = (seq_t)4;

    /* Reduce the fraction by dividing out the gcd of |numer| and 4. */
    seq_t abs_numer = numer >= 0 ? numer : -numer;
    seq_t g = gcd(abs_numer, denom);

    /* Return the reduced numerator. */
    return numer / g;
}
