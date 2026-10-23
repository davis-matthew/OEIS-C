/*
 * A016176  Inverse of 2167th cyclotomic polynomial.
 * https://oeis.org/A016176
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in the power‑series * 1 / Φ_{2167}(x),
 *            * where Φ_{2167}(x) = (x^{2168} - x^{2167} - x + 1) * / (x^{208}
 *            - x^{197} - x^{11} + 1). * The sequence is periodic with period
 *            2167; hence * a(n) = coeffB[n % 2167] where coeffB holds the
 *            first 2167 * coefficients of the formal in...
 * Range:     i8 n <= 3000, i16 n <= 3000, i32 n <= 3000, i64 n <= 3000,
 *            i128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard includes. */
#include <stdlib.h>
#include <stdint.h>

/* The types seq_t and idx_t are supplied by the build system. */

/* The function required by the specification. */
seq_t a(idx_t n)
{
    /* Period length of the sequence. */
    static const idx_t PERIOD = 2167U;

    /* One-time initialization flag and storage. */
    static idx_t init = 0;
    static seq_t *coeffA = NULL;   /* coefficients of Φ_{2167}(x), size 1961 */
    static seq_t *coeffB = NULL;   /* first PERIOD coefficients of 1/Φ_{2167}(x) */

    if (!init)
    {
        /* Allocate arrays for the numerator, denominator, quotient,
         * and the inverse series.  All indices are non‑negative. */
        const idx_t DEG_NUM   = 2168U;        /* degree of the numerator polynomial */
        const idx_t DEG_DEN   =  208U;        /* degree of the denominator polynomial */
        const idx_t DEG_COEFF = DEG_NUM - DEG_DEN; /* degree of Φ_{2167}(x) = 1960 */

        /* Allocate temporary arrays. */
        seq_t *num = (seq_t *)malloc((DEG_NUM + 1) * sizeof(seq_t));
        seq_t *den = (seq_t *)malloc((DEG_DEN + 1) * sizeof(seq_t));
        coeffA = (seq_t *)malloc((DEG_COEFF + 1) * sizeof(seq_t));

        /* --------- Build the numerator: x^2168 - x^2167 - x + 1 --------- */
        for (idx_t i = 0; i <= DEG_NUM; ++i)
            num[i] = 0;
        num[0]   = 1; /* constant term */
        num[1]   = -1;/* -x */
        num[2167] = -1;/* -x^2167 */
        num[2168] = 1; /* +x^2168 */

        /* --------- Build the denominator: x^208 - x^197 - x^11 + 1 --------- */
        for (idx_t i = 0; i <= DEG_DEN; ++i)
            den[i] = 0;
        den[0]   = 1;   /* constant term */
        den[11]  = -1;  /* -x^11 */
        den[197] = -1;  /* -x^197 */
        den[208] = 1;   /* +x^208 */

        /* --------- Polynomial long division: N(x) / D(x) --------- */
        /* Because the leading coefficient of D(x) is 1, we can divide by
         * simply subtracting the appropriate multiple of D(x). */
        for (idx_t k = DEG_NUM; k >= DEG_DEN; --k)
        {
            seq_t coeff = num[k];
            if (coeff != 0)
            {
                /* quotient coefficient at degree k - DEG_DEN */
                coeffA[k - DEG_DEN] = coeff;

                /* Subtract coeff * D(x) shifted to align with x^k */
                for (idx_t j = 0; j <= DEG_DEN; ++j)
                {
                    num[k - DEG_DEN + j] -= coeff * den[j];
                }
            }
        }

        /* --------- Compute the inverse series B(x) up to order PERIOD-1 --------- */
        coeffB = (seq_t *)malloc(PERIOD * sizeof(seq_t));
        coeffB[0] = 1; /* A(0) = 1, so B(0) = 1 / A(0) = 1 */

        for (idx_t k = 1; k < PERIOD; ++k)
        {
            seq_t sum = 0;
            /* The recurrence: for k > 0,
             * 0 = sum_{i=0}^{k} a_i * b_{k-i}
             * with a_0 = 1, so b_k = -sum_{i=1}^{min(k,DEG_COEFF)} a_i * b_{k-i}. */
            idx_t max_i = (k < (idx_t)DEG_COEFF) ? k : (idx_t)DEG_COEFF;
            for (idx_t i = 1; i <= max_i; ++i)
            {
                sum += coeffA[i] * coeffB[k - i];
            }
            coeffB[k] = -sum;
        }

        free(num);
        free(den);

        init = 1;
    }

    /* Return the periodic sequence value. */
    idx_t pos = n % PERIOD;
    return coeffB[pos];
}
