/*
 * A014826  Inverse of 817th cyclotomic polynomial.
 * https://oeis.org/A014826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the coefficient of x^n in the power series expansion of
 *            1/Φ_817(x) (inverse of the 817th cyclotomic polynomial). The
 *            series is periodic with period 817. The coefficient a(n) for n≥0
 *            is obtained by computing the coefficients of Φ_817(x) once,
 *            using them in the recurrence a(0) = 1 a(n) = -∑...
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *phi_coeff = NULL;     /* c_k of Φ_817(x), length 757 */
    static seq_t *a_vals   = NULL;     /* a(i) for i = 0..816, length 817 */

    if (phi_coeff == NULL) {
        /* ------------------------------------------
           Compute coefficients of Φ_817(x)
        ------------------------------------------ */
        const size_t num_deg = 818;          /* degree of (x-1)(x^817-1)   */
        const size_t den_deg = 62;           /* degree of (x^19-1)(x^43-1) */
        const size_t q_deg  = num_deg - den_deg;   /* 756 */

        seq_t *num   = (seq_t*)malloc((num_deg + 1) * sizeof(seq_t));
        seq_t *den   = (seq_t*)malloc((den_deg + 1) * sizeof(seq_t));
        seq_t *temp  = (seq_t*)malloc((num_deg + 1) * sizeof(seq_t));

        /* initialise to zero */
        for (size_t i = 0; i <= num_deg;  i++) num[i]  = 0;
        for (size_t i = 0; i <= den_deg;  i++) den[i]  = 0;
        for (size_t i = 0; i <= num_deg;  i++) temp[i] = 0;

        /* (x-1)(x^817-1) = x^818 - x^817 - x + 1 */
        num[0] = 1;
        num[1] = -1;
        num[817] = -1;
        num[818] = 1;

        /* (x^19-1)(x^43-1) = x^62 - x^43 - x^19 + 1 */
        den[0]  = 1;
        den[19] = -1;
        den[43] = -1;
        den[62] = 1;

        /* copy numerator to temp for division */
        for (size_t i = 0; i <= num_deg; i++) temp[i] = num[i];

        phi_coeff = (seq_t*)malloc((q_deg + 1) * sizeof(seq_t));

        for (size_t i = 0; i <= q_deg; i++) {
            seq_t coeff = temp[i];                /* denom[0] = 1 */
            phi_coeff[i] = coeff;
            if (coeff != 0) {
                for (size_t j = 0; j <= den_deg; j++) {
                    temp[i + j] -= coeff * den[j];
                }
            }
        }

        /* ------------------------------------------
           Compute the first 817 terms of a(n)
        ------------------------------------------ */
        a_vals = (seq_t*)malloc(817 * sizeof(seq_t));
        a_vals[0] = 1;   /* a(0) = 1 */

        for (size_t n_idx = 1; n_idx < 817; n_idx++) {
            seq_t sum = 0;
            size_t kmax = (n_idx <= q_deg) ? n_idx : q_deg;
            for (size_t k = 1; k <= kmax; k++) {
                sum += a_vals[n_idx - k] * phi_coeff[k];
            }
            a_vals[n_idx] = -sum;
        }

        /* free temporary arrays */
        free(num);
        free(den);
        free(temp);
    }

    /* the sequence is periodic of length 817 */
    size_t idx = (size_t)(n % 817);
    return a_vals[idx];
}
