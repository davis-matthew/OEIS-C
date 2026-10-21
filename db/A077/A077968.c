/*
 * A077968  Expansion of 1/(1+2*x^2+2*x^3).
 * https://oeis.org/A077968
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=1, a(1)=0, a(2)=-2, a(n) = -2*a(n-2)-2*a(n-3) for n≥3
 *            (Expansion of 1/(1+2*x^2+2*x^3))
 * Range:     i8 n <= 11, i16 n <= 22, i32 n <= 46, i64 n <= 91, i128 n <= 185,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *coeff = NULL;
    static size_t computed = 0;   /* number of terms already allocated */

    size_t need = (size_t)n + 1;
    if (need > computed) {
        seq_t *tmp = (seq_t *)realloc(coeff, need * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – clean up and return a safe value */
            if (coeff) free(coeff);
            coeff = NULL;
            computed = 0;
            return (seq_t)0;
        }
        coeff = tmp;

        for (size_t i = computed; i < need; ++i) {
            if (i == 0)
                coeff[i] = (seq_t)1;
            else if (i == 1)
                coeff[i] = (seq_t)0;
            else if (i == 2)
                coeff[i] = (seq_t)-2;
            else
                coeff[i] = (seq_t)-2 * coeff[i-2] + (seq_t)-2 * coeff[i-3];
        }
        computed = need;
    }

    return coeff[(size_t)n];
}
