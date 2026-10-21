/*
 * A212234  Number of 0..2 arrays of length 1+2*n with sum no more than 2*n in
 *          any length 2n subsequence (=50% duty cycle).
 * https://oeis.org/A212234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^(2*n)] (1+z+z^2)^(2*n-1)*(1+3*z+5*z^2)/(1-z)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes the coefficients of (1+z+z^2)^(2*n-1)
   only up to degree 2*n and then evaluates the defined closed form. */
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Offset 1: n >= 1 */
    if (n == 0) return 0;   /* defined only for n>=1, but guard anyway */

    /* Parameters for the generating function */
    idx_t m       = 2 * n - 1;      /* exponent in (1+z+z^2)^m  */
    idx_t max_k   = 2 * n;          /* we need coefficients up to degree 2*n */

    /* Dynamically allocate array for coefficients [0..max_k] */
    seq_t *coeff = (seq_t *)malloc((max_k + 1) * sizeof(seq_t));
    if (!coeff)
    {
        /* If allocation fails, return 0 as a harmless fallback */
        return 0;
    }
    memset(coeff, 0, (max_k + 1) * sizeof(seq_t));
    coeff[0] = 1;   /* (1+z+z^2)^0 = 1 */

    /* Build the polynomial (1+z+z^2)^m iteratively */
    for (idx_t step = 0; step < m; ++step)
    {
        /* After 'step' multiplications, the maximum possible degree is 2*step.
           We stop at min(2*step, max_k) to avoid computing unnecessary terms. */
        idx_t limit = (step + 1) * 2;
        if (limit > max_k) limit = max_k;

        /* Update coefficients backwards so that the previous values are still
           available for the lower indices. */
        for (idx_t d = limit; ; --d)
        {
            seq_t sum = coeff[d];
            if (d >= 1) sum += coeff[d - 1];
            if (d >= 2) sum += coeff[d - 2];
            coeff[d] = sum;

            if (d == 0) break;
        }
    }

    /* Sum coefficients up to 2*n-2 (i.e., up to max_k-2) */
    seq_t prefix = 0;
    if (max_k >= 2)   /* for n >= 1, max_k is at least 2 */
    {
        for (idx_t k = 0; k <= max_k - 2; ++k)
            prefix += coeff[k];
    }

    /* Compute the value according to the closed form:
       a(n) = c_{2n} + 4*c_{2n-1} + 9*sum_{k=0}^{2n-2} c_k
       where c_k are the coefficients of (1+z+z^2)^(2*n-1). */
    seq_t result = coeff[max_k] + 4u * coeff[max_k - 1] + 9u * prefix;

    free(coeff);
    return result;
}
