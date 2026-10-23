/*
 * A015815  Inverse of 1806th cyclotomic polynomial.
 * https://oeis.org/A015815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, changed
 * Formula:   not stated
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Coefficients of the formal power series expansion of 1/Φ_1806(x). */
/* They are periodic with period 1806. */
#include <stdbool.h>
#include <stdint.h>

#define PERIOD 1806/* period length */

static bool constructed = false;
static seq_t coeff[PERIOD];

seq_t a(idx_t n)
{
    if (!constructed) {
        /* initialize all coefficients to 0 except coeff[0] = 1  */
        for (idx_t i = 0; i < PERIOD; ++i) coeff[i] = 0;
        coeff[0] = 1;

        /* primes dividing 1806 */
        const uint32_t primes[4] = {2, 3, 7, 43};
        const int    primeCnt   = 4;

        /* iterate over all divisors of 1806 using subsets of primes */
        for (int mask = 0; mask < (1 << primeCnt); ++mask) {
            idx_t d = 1;
            int  bits = 0;
            for (int k = 0; k < primeCnt; ++k) {
                if (mask & (1 << k)) {
                    d *= primes[k];
                    ++bits;
                }
            }
            /* complement size determines the exponent sign for (1 - x^d)                     */
            int compSize = primeCnt - bits;             /* number of primes not in d  */
            bool infiniteSeries = (compSize % 2 == 0);   /* μ(n/d) = +1 -> exponent -1 (1/(1-x^d))   */

            if (infiniteSeries) {          /* multiplicative factor: 1/(1-x^d)  */
                for (idx_t i = d; i < PERIOD; ++i) {
                    coeff[i] += coeff[i - d];
                }
            } else {                       /* multiplicative factor: (1 - x^d)  */
                for (idx_t i = PERIOD; i-- > d; ) {
                    coeff[i] -= coeff[i - d];
                }
            }
        }
        constructed = true;
    }
    return coeff[n % PERIOD];
}
