/*
 * A015985  Inverse of 1976th cyclotomic polynomial.
 * https://oeis.org/A015985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in 1/Φ_1976(x). The sequence is
 *            periodic with period 1976. We compute one period at install time
 *            and return the appropriate coefficient
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* malloc, free */
#include <stddef.h>  /* size_t */

seq_t a(idx_t n)
{
    const size_t P = 1976;
    static seq_t *period = NULL;
    static size_t period_computed = 0;

    if (!period_computed) {
        /* Temporary arrays for the convolution.  The casts are required
           for C++ compatibility; they are valid in C as well. */
        seq_t *cur  = (seq_t*)malloc(sizeof(seq_t) * P);
        seq_t *next = (seq_t*)malloc(sizeof(seq_t) * P);
        if (!cur || !next) {
            if (cur) free(cur);
            if (next) free(next);
            return 0;                     /* allocation failure → 0 */
        }

        /* Initialise cur to 1 followed by zeros. */
        for (size_t i = 0; i < P; ++i) cur[i] = 0;
        cur[0] = 1;

        /* Factors arising from the Möbius inversion for Φ_1976. */
        const size_t factor_d[]   = {1976, 988, 152, 104,  76,  52,   8,   4};
        const int    factor_exp[] = {-1,   1,   1,   1,  -1, -1,  -1,   1};
        const size_t num_factors  = sizeof(factor_d) / sizeof(factor_d[0]);

        for (size_t f = 0; f < num_factors; ++f) {
            size_t d   = factor_d[f];
            int    exp = factor_exp[f];

            if (exp == 1) {                /* multiply by (1 - x^d) */
                for (size_t i = 0; i < P; ++i) {
                    seq_t val = cur[i];
                    if (i >= d) val -= cur[i - d];
                    next[i] = val;
                }
            } else {                       /* exp == -1 : multiply by
                                               (1 - x^d)^(-1) = Σ_{m>=0} x^{m d} */
                for (size_t i = 0; i < P; ++i) {
                    seq_t sum = 0;
                    size_t m = 0;
                    while (1) {
                        size_t idx = i - m * d;
                        sum += cur[idx];
                        if (i < (m + 1) * d) break;
                        ++m;
                    }
                    next[i] = sum;
                }
            }

            /* Swap cur and next for the next factor. */
            seq_t *tmp = cur;
            cur = next;
            next = tmp;
        }

        /* Store the computed period. */
        period = (seq_t*)malloc(sizeof(seq_t) * P);
        if (!period) {
            free(cur);
            free(next);
            return 0;
        }
        for (size_t i = 0; i < P; ++i) period[i] = cur[i];

        free(cur);
        free(next);

        period_computed = 1;
    }

    /* Return the coefficient corresponding to n, modulo the period. */
    size_t idx = (size_t)(n % P);
    return period[idx];
}
