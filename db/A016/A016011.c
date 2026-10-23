/*
 * A016011  Inverse of 2002nd cyclotomic polynomial.
 * https://oeis.org/A016011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 3000, i16 n <= 3000, i32 n <= 3000, i64 n <= 3000,
 *            i128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A016011: coefficients of the formal series 1/Φ2002(x),
 * where Φ2002(x) is the 2002‑th cyclotomic polynomial.
 * The sequence is periodic with period 2002:
 *      a(n+2002) = a(n)   for all n >= 0.
 */

#include <stdbool.h>
#include <stddef.h>

#define PERIOD 2002

seq_t a(idx_t n)
{
    static seq_t coeff[PERIOD];
    static bool initialized = false;

    if (!initialized) {
        /* start with the constant series 1 */
        for (int i = 0; i < PERIOD; ++i)
            coeff[i] = 0;
        coeff[0] = 1;

        /* primes dividing 2002 = 2 * 7 * 11 * 13 */
        const idx_t primes[4] = {2, 7, 11, 13};

        /* for every divisor d of 2002, compute e(d) = -μ(2002/d) */
        for (int mask = 0; mask < (1 << 4); ++mask) {
            /* d is the product of primes selected by mask */
            idx_t d = 1;
            int bits = 0;          /* number of selected primes = #primes in d */
            for (int p = 0; p < 4; ++p) {
                if (mask & (1 << p)) {
                    d *= primes[p];
                    ++bits;
                }
            }

            /* e(d) = +1 if bits is odd, -1 if bits is even */
            int e = (bits & 1) ? 1 : -1;
            int d_int = (int)d;   /* safe: d <= 2002 */

            if (e == 1) {
                /* multiply by (x^d - 1):   new[k] = coeff[k] - coeff[k-d] */
                seq_t newcoeff[PERIOD];
                for (int k = 0; k < PERIOD; ++k) {
                    newcoeff[k] = coeff[k];
                    if (k >= d_int)
                        newcoeff[k] -= coeff[k - d_int];
                }
                for (int k = 0; k < PERIOD; ++k)
                    coeff[k] = newcoeff[k];
            } else {
                /* multiply by 1/(1 - x^d) = sum_{j>=0} x^{j d} */
                seq_t newcoeff[PERIOD];
                for (int k = 0; k < PERIOD; ++k) {
                    if (k >= d_int)
                        newcoeff[k] = newcoeff[k - d_int] + coeff[k];
                    else
                        newcoeff[k] = coeff[k];
                }
                for (int k = 0; k < PERIOD; ++k)
                    coeff[k] = newcoeff[k];
            }
        }
        initialized = true;
    }

    return coeff[(size_t)(n % PERIOD)];
}
