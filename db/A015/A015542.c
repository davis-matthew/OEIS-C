/*
 * A015542  Inverse of 1533rd cyclotomic polynomial.
 * https://oeis.org/A015542
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

/*  A015542  –  inverse of the 1533‑rd cyclotomic polynomial.
 *  The sequence is periodic with period 1533.
 *  a(n) is the coefficient of x^n in the power series expansion of
 *  1/Φ_1533(x), where Φ_1533(x) is the 1533‑th cyclotomic polynomial.
 *
 *  The implementation pre‑computes the coefficients for one period
 *  using successive convolutions, then returns the requested term
 *  by reducing the index modulo 1533.
 */

#include <stdlib.h>
#include <stdbool.h>

#define PERIOD 1533

/*  Public entry point required by the build system.  */
seq_t a(idx_t n)
{
    /*  Static storage for the period coefficients.\n   */
    static seq_t *coef   = NULL;
    static bool       init = false;

    if (!init) {
        /*  Allocate the coefficient array for the period.  */
        coef = (seq_t *)malloc(PERIOD * sizeof(seq_t));
        if (!coef) return 0;   /*  unlikely failure  */

        /*  Temporary arrays for successive convolutions.  */
        seq_t *vec  = (seq_t *)calloc(PERIOD, sizeof(seq_t));
        seq_t *temp = NULL;

        if (!vec) {
            free(coef);
            return 0;
        }

        vec[0] = 1;   /*  start with the series ‘1’  */

        /*  List of factors in the representation
         *  1/Φ_1533(x) = ∏(x^r - 1)^{e_r}
         *  where e = +1 or –1 as given below.  */
        struct factor { int r; int e; };   /*  r > 0  */
        const struct factor factors[] = {
            { 1,  +1 },   /*  (x - 1)       */
            { 3,  -1 },   /*  1/(x^3 - 1)   */
            { 7,  -1 },   /*  1/(x^7 - 1)   */
            {21,  +1 },   /*  (x^21 - 1)    */
            {73,  -1 },   /*  1/(x^73 - 1)  */
            {219, +1 },   /*  (x^219 - 1)   */
            {511, +1 },   /*  (x^511 - 1)   */
            {1533,+1 }    /*  (x^1533 - 1)  */
        };
        const int nfactors = sizeof(factors)/sizeof(factors[0]);

        /*  Successively multiply the current series by each factor.\n   */
        for (int fi = 0; fi < nfactors; ++fi) {
            const int r = factors[fi].r;
            const int e = factors[fi].e;

            temp = (seq_t *)calloc(PERIOD, sizeof(seq_t));
            if (!temp) {
                free(vec); free(coef);
                return 0;
            }

            if (e == +1) {            /*  multiply by (1 - x^r)  */
                for (int i = 0; i < PERIOD; ++i) {
                    temp[i] = vec[i];
                    if (i >= r) temp[i] -= vec[i - r];
                }
            } else {                  /*  multiply by 1/(1 - x^r)  */
                for (int i = 0; i < PERIOD; ++i) {
                    seq_t v = vec[i];
                    if (v == 0) continue;
                    for (int j = i; j < PERIOD; j += r) {
                        temp[j] += v;
                    }
                }
            }

            free(vec);
            vec = temp;
        }

        /*  Store the pre‑computed period into the static cache.  */
        for (int i = 0; i < PERIOD; ++i)
            coef[i] = vec[i];

        free(vec);
        init = true;
    }

    /*  Return the coefficient with periodic indexing.  */
    return coef[(size_t)n % PERIOD];
}
