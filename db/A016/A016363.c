/*
 * A016363  238th cyclotomic polynomial.
 * https://oeis.org/A016363
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of x^n in the 238th cyclotomic polynomial
 *            Φ_238(x)
 * Range:     i8 n <= 96, i16 n <= 96, i32 n <= 96, i64 n <= 96, i128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t *coeffs = NULL;
    static idx_t degree = 0;

    if (!initialized) {
        const idx_t N = 238;          /* n of the cyclotomic polynomial */
        /* positive μ divisors: d = 1, 14, 34, 119  -> m = N/d */
        const idx_t pos_m[4] = {238, 17, 7, 2};
        /* negative μ divisors: d = 2, 7, 17, 238 -> m = N/d */
        const idx_t neg_m[4] = {119, 34, 14, 1};

        /* start with polynomial 1 */
        seq_t *poly = (seq_t*)malloc(sizeof(seq_t));
        poly[0] = 1;
        idx_t deg = 0;

        /* multiplication by (x^m - 1) for all positive exponents */
        for (int i = 0; i < 4; ++i) {
            idx_t m = pos_m[i];
            idx_t new_deg = deg + m;
            seq_t *new_poly = (seq_t*)calloc(new_deg + 1, sizeof(seq_t));

            for (idx_t j = 0; j <= new_deg; ++j) {
                seq_t val = 0;
                if (j >= m && j - m <= deg) val += poly[j - m];
                if (j <= deg)                     val -= poly[j];
                new_poly[j] = val;
            }
            free(poly);
            poly = new_poly;
            deg  = new_deg;
        }

        /* division by (x^m - 1) for all negative exponents */
        for (int i = 0; i < 4; ++i) {
            idx_t m = neg_m[i];
            if (m > deg) { /* should never happen for 238 */ }
            seq_t *quot = (seq_t*)calloc(deg - m + 1, sizeof(seq_t));

            for (int64_t j = (int64_t)deg; j >= (int64_t)m; --j) {
                seq_t qcoeff = poly[(idx_t)j];
                if (qcoeff != 0) {
                    quot[(idx_t)(j - m)] = qcoeff;
                    poly[(idx_t)(j - m)] += qcoeff;   /* add qcoeff */
                    poly[(idx_t)j]       -= qcoeff;   /* subtract qcoeff */
                }
            }
            deg -= m;
            free(poly);
            poly = quot;
        }

        /* poly now contains the coefficients of Φ_238(x) */
        degree = deg;          /* should be 96 */
        coeffs = poly;
        initialized = true;
    }

    if (n <= degree)
        return coeffs[n];
    return (seq_t)0;
}
