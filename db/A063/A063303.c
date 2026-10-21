/*
 * A063303  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          30 ).
 * https://oeis.org/A063303
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is the coefficient of x^n in
 *            -x^2*(x^9-9*x^7-3*x^6-5*x^5-13*x^4-3*x^3-5*x^2-8*x+1) /
 *            ((x-1)^2*(x+1)^2*(x^2-x+1)*(x^2+1)) This yields a linear
 *            recurrence of order 8: a(n) = a(n-1) - a(n-3) + 2*a(n-4) -
 *            a(n-5) + a(n-7) - a(n-8) (n≥12) with initial values from a(2) to
 *            a(11). The code below comp...
 * Range:     i8 n <= 24, i16 n <= 54, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/grow_seq_array.h"

static const int q[9] = {1,-1,0,1,-2,1,0,-1,1};          /* coefficients of Q(x) */
static const seq_t p_coeff[12] = {0,0,-1,8,5,3,13,5,3,9,0,-1}; /* coefficients of P(x) */

static seq_t *coeff = NULL;          /* dynamic array of computed coefficients */
static size_t coeff_len = 0;          /* allocated length of coeff[]        */
static idx_t computed_max = (idx_t)-1;/* largest index for which coeff[] is set */

/* Return the value a(n) for n ≥ 2. For n < 2 the value is undefined;
   the function simply returns 0 in that case. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;           /* offset 2; behaviour below is irrelevant */

    /* ensure the array is large enough */
    size_t need = (size_t)n + 1;
    if (coeff_len < need) {
        size_t old_len = coeff_len;
        grow_seq_array(&coeff, &coeff_len, need);
        for (size_t i = old_len; i < coeff_len; ++i) coeff[i] = 0;
    }

    /* compute new coefficients if necessary */
    size_t start = 0;
    if (computed_max != (idx_t)-1) start = (size_t)computed_max + 1;

    for (size_t i = start; i <= (size_t)n; ++i) {
        seq_t sum = 0;
        if (i <= 11) sum = p_coeff[i];            /* p[i] is non‑zero only for i≤11 */
        for (int j = 1; j <= 8; ++j) {
            if (i >= (size_t)j)
                sum -= (seq_t)q[j] * coeff[i-j];
        }
        coeff[i] = sum;
    }

    computed_max = n;
    return coeff[n];
}
