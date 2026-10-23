/*
 * A112182  McKay-Thompson series of class 40d for the Monster group.
 * https://oeis.org/A112182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of chi(-x)*chi(-x^5), where chi(q) =
 *            product_{m>=1} (1 + q^m). Equivalently, A(x) = ∏_{odd m≥1} (1 -
 *            x^m)^{c_m}, with c_m = 1 if m not divisible by 5, 2 if m
 *            divisible by 5. a(n) = the coefficient of x^n in this product
 * Range:     i8 n <= 42, i16 n <= 121, i32 n <= 380, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* seq_t and idx_t are defined by the build. */

static seq_t *coeff = NULL;   /* current coefficient array a[0..max_N] */
static idx_t   max_N = 0;     /* maximum index that has been computed */

/* Compute all coefficients a[0..N] using the product representation. */
static void compute_up_to(idx_t N)
{
    /* Allocate new array */
    seq_t *new_coeff = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!new_coeff) return;            /* allocation failure: propagate NULL */

    /* Initialize to zero */
    for (idx_t i = 0; i <= N; ++i) new_coeff[i] = 0;
    new_coeff[0] = 1;                  /* a(0) = 1 */

    /* Multiply by each factor (1 - x^m) or (1 - x^m)^2 */
    for (idx_t m = 1; m <= N; ++m) {
        if ((m & 1) == 0) continue;    /* skip even m : exponent 0 */
        if (m % 5 == 0) {              /* exponent 2: (1 - x^m)^2 */
            for (idx_t k = N; k >= m; --k) {
                /* new_coeff[k] += -2 * new_coeff[k-m]  */
                new_coeff[k] -= (seq_t)2 * new_coeff[k - m];
                if (k >= 2 * m) {
                    /* new_coeff[k] += new_coeff[k-2*m]    */
                    new_coeff[k] += new_coeff[k - 2 * m];
                }
            }
        } else {                       /* exponent 1: (1 - x^m)   */
            for (idx_t k = N; k >= m; --k) {
                new_coeff[k] -= new_coeff[k - m];
            }
        }
    }

    /* Replace old array with new one */
    free(coeff);
    coeff = new_coeff;
    max_N  = N;
}

/* External function required by the test harness */
seq_t a(idx_t n)
{
    /* Compute up to n if necessary */
    if (coeff == NULL || n > max_N) {
        compute_up_to(n);
        if (coeff == NULL) return (seq_t)0;   /* allocation failed */
    }
    return coeff[n];
}
