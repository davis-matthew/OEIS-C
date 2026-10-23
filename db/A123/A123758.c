/*
 * A123758  Expansion of q^(-1/3)*eta(q)*eta(q^4)*eta(q^5)/eta(q^2) in powers
 *          of q.
 * https://oeis.org/A123758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the coefficient of q^n in
 *     ∏_{k>0} (1 - q^k)(1 + q^{2k})(1 - q^{5k})
 *
 * The function builds the coefficients on demand in O(n^2) time
 * for the largest n requested so far and caches the result for
 * subsequent calls.
 */
#include <stdlib.h>
#include <stdint.h>

static seq_t *coeff = NULL;      /* cached coefficients */
static idx_t coeff_size = 0;     /* number of entries stored (coeff_size = max_n+1) */

/* recompute all coefficients up to n (inclusive) */
static void recompute_coeff(idx_t n)
{
    seq_t *newcoeff = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!newcoeff)
        exit(EXIT_FAILURE);   /* cannot allocate */

    /* initialise to zero */
    for (idx_t i = 0; i <= n; ++i)
        newcoeff[i] = 0;
    newcoeff[0] = 1;           /* empty product contributes 1 */

    /* factor (1 - q^k)  */
    for (idx_t k = 1; k <= n; ++k) {
        for (idx_t i = n; i >= k; --i)
            newcoeff[i] -= newcoeff[i - k];
    }

    /* factor (1 + q^{2k})  */
    for (idx_t k = 1; k <= n / 2; ++k) {
        for (idx_t i = n; i >= 2 * k; --i)
            newcoeff[i] += newcoeff[i - 2 * k];
    }

    /* factor (1 - q^{5k})  */
    for (idx_t k = 1; k <= n / 5; ++k) {
        for (idx_t i = n; i >= 5 * k; --i)
            newcoeff[i] -= newcoeff[i - 5 * k];
    }

    /* replace old array */
    free(coeff);
    coeff      = newcoeff;
    coeff_size = n + 1;
}

seq_t a(idx_t n)
{
    /* compute up to requested n if necessary */
    if (n >= coeff_size)
        recompute_coeff(n);
    return coeff[n];
}
