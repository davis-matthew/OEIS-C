/*
 * A016253  Inverse of 2244th cyclotomic polynomial.
 * https://oeis.org/A016253
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, changed
 * Formula:   not stated
 * Range:     i8 n <= 3000, i16 n <= 3000, i32 n <= 3000, i64 n <= 3000,
 *            i128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Inverse of the 2244th cyclotomic polynomial.
   a(n) is the coefficient of x^n in the power‑series expansion of
   1 / Φ_2244(x).  The sequence is periodic with period 2244. */

#include <stddef.h>          /* for size_t (we use it in the return indexing) */

#define PERIOD 2244          /* period length (compile‑time constant) */

seq_t a(idx_t n)
{
    /* static cache of the periodic coefficients */
    static seq_t coeff[PERIOD] = { 0 };   /* zero‑initialized automatically */
    static int initialized = 0;

    if (!initialized) {
        /* a(0) = 1 is specified in the OEIS sequence */
        coeff[0] = (seq_t)1;

        /* Enumerate all divisors of PERIOD = 2²·3·11·17 */
        for (int e2 = 0; e2 <= 2; ++e2) {
            for (int e3 = 0; e3 <= 1; ++e3) {
                for (int e11 = 0; e11 <= 1; ++e11) {
                    for (int e17 = 0; e17 <= 1; ++e17) {
                        int d = 1;
                        if (e2) d <<= e2;                /* multiply by 2^e2 */
                        d *= (e3  ? 3 : 1);
                        d *= (e11 ? 11 : 1);
                        d *= (e17 ? 17 : 1);

                        /* Compute μ(m) where m = PERIOD / d */
                        int m = PERIOD / d;
                        int temp = m;
                        int exp2 = 0, exp3 = 0, exp11 = 0, exp17 = 0;

                        /* factor out 2 */
                        while ((temp & 1) == 0) { ++exp2; temp >>= 1; }
                        /* factor out 3 */
                        while (temp % 3 == 0) { ++exp3; temp /= 3; }
                        /* factor out 11 */
                        while (temp % 11 == 0) { ++exp11; temp /= 11; }
                        /* factor out 17 */
                        while (temp % 17 == 0) { ++exp17; temp /= 17; }

                        int mu;
                        if (exp2 > 1 || exp3 > 1 || exp11 > 1 || exp17 > 1)
                            mu = 0;                      /* μ(m) = 0 if any prime power > 1 */
                        else {
                            int primes_present = (exp2 > 0) + (exp3 > 0)
                                                + (exp11 > 0) + (exp17 > 0);
                            mu = (primes_present % 2 == 0) ? 1 : -1;
                        }

                        /* Apply the factor (1 - x^d)^(-μ(m)) */
                        if (mu == 1) {   /* multiply by 1/(1 - x^d) */
                            for (int i = d; i < PERIOD; ++i)
                                coeff[i] += coeff[i - d];
                        } else if (mu == -1) { /* multiply by (1 - x^d) */
                            for (int i = PERIOD - 1; i >= d; --i)
                                coeff[i] -= coeff[i - d];
                        }
                    }
                }
            }
        }
        initialized = 1;
    }

    /* Return the periodic term a(n) = coeff[n % PERIOD] */
    return coeff[(size_t)(n % PERIOD)];
}
