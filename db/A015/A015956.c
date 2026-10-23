/*
 * A015956  Inverse of 1947th cyclotomic polynomial.
 * https://oeis.org/A015956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = coefficient of x^n in 1/Φ_1947(x) (inverse of the
 *   1947th cyclotomic polynomial).  The sequence satisfies a(0)=1 and
 *   a(k) = - Σ_{i=1}^{k} φ_i * a(k-i) where φ_i are coefficients of
 *   Φ_1947(x).  It is periodic with period 1947.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Compile‑time constants */
    enum {
        PERIOD = 1947,
        MAXDEG = 2100,
        POS_DIV_CNT = 4,
        NEG_DIV_CNT = 4
    };

    /* Static cache of the periodic values */
    static seq_t period_values[PERIOD];
    static bool initialized = false;

    if (!initialized) {
        /* Working arrays for polynomial manipulation */
        seq_t poly[MAXDEG];
        seq_t temp[MAXDEG];
        idx_t deg = 0;

        /* Initialise to the constant polynomial 1 */
        for (idx_t i = 0; i < MAXDEG; ++i) poly[i] = (seq_t)0;
        poly[0] = (seq_t)1;

        /* Positively signed divisors (μ(1947/d) = +1) */
        const idx_t pos_div[POS_DIV_CNT] = {3, 11, 59, 1947};
        for (size_t t = 0; t < POS_DIV_CNT; ++t) {
            idx_t d = pos_div[t];
            idx_t new_deg = deg + d;
            for (idx_t m = 0; m <= new_deg; ++m) {
                seq_t val = 0;
                if (m >= d && m - d <= deg) val += poly[m - d];
                if (m <= deg) val -= poly[m];
                temp[m] = val;
            }
            for (idx_t m = 0; m <= new_deg; ++m) poly[m] = temp[m];
            deg = new_deg;
        }

        /* Negatively signed divisors (μ(1947/d) = -1) */
        const idx_t neg_div[NEG_DIV_CNT] = {1, 33, 177, 649};
        for (size_t t = 0; t < NEG_DIV_CNT; ++t) {
            idx_t d = neg_div[t];
            idx_t new_deg = deg - d;   /* division keeps degree smaller */
            for (idx_t m = 0; m <= new_deg; ++m) {
                if (m < d) {
                    temp[m] = -poly[m];
                } else {
                    temp[m] = -poly[m] + temp[m - d];
                }
            }
            for (idx_t m = 0; m <= new_deg; ++m) poly[m] = temp[m];
            deg = new_deg;
        }

        /* Compute a[0 .. PERIOD-1] using the recurrence */
        seq_t a_seq[PERIOD];
        a_seq[0] = (seq_t)1;
        for (idx_t k = 1; k < PERIOD; ++k) {
            seq_t sum = (seq_t)0;
            idx_t max_i = (k <= deg) ? k : deg;
            for (idx_t i = 1; i <= max_i; ++i) {
                sum += poly[i] * a_seq[k - i];
            }
            a_seq[k] = -sum;
        }

        /* Store into the cache */
        for (idx_t i = 0; i < PERIOD; ++i) period_values[i] = a_seq[i];
        initialized = true;
    }

    return period_values[n % PERIOD];
}
