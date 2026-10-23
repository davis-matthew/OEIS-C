/*
 * A015206  Inverse of 1197th cyclotomic polynomial.
 * https://oeis.org/A015206
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   A015206 – inverse of the 1197th cyclotomic polynomial. The
 *            sequence is periodic with period 1197. It satisfies a(0)=1 and
 *            for n>0 a(n) = -∑_{k=1}^{648} c_k * a(n-k), where c_k are the
 *            integer coefficients of Φ₁₁₉₇(x)
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   compute_cyclotomic_coeffs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/compute_cyclotomic_coeffs.h"

/* Period of the sequence. */
#define PERIOD 1197

/* Main function implementing A015206. */
seq_t a(idx_t n)
{
    static int initialized = 0;
    static seq_t cache[PERIOD];
    static seq_t coeff[649]; /* coeff[1..648] are used; coeff[0] unused */

    if (!initialized) {
        /* Precompute Φ₁₁₉₇ coefficients. */
        compute_cyclotomic_coeffs(coeff);

        /* Build the periodic sequence using the recurrence. */
        cache[0] = 1;                 /* a(0) = 1 */
        for (idx_t i = 1; i < PERIOD; ++i) {
            seq_t sum = 0;
            for (idx_t k = 1; k <= 648 && i >= k; ++k) {
                sum += coeff[k] * cache[i - k];
            }
            cache[i] = -sum;
        }
        initialized = 1;
    }
    return cache[n % PERIOD];
}
