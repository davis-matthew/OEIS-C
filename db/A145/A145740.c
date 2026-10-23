/*
 * A145740  McKay-Thompson series of class 20C for the Monster group with a(0)
 *          = -2.
 * https://oeis.org/A145740
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 41, i16 n <= 131, i32 n <= 441, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A145740
    Coefficients of the series

       G(q) = ( eta(q) * eta(q^4) * eta(q^10) /
                ( eta(q^2) * eta(q^5) * eta(q^20) ) )^2

    Indexed with offset -1 (a(-1) is the first term).
    The series can be written as an infinite product

        ∏_{k=1..∞} (1 - q^k)^{e(k)}

    where e(k) is 2 if k is not divisible by 2 or 5,
    or k is divisible by 4 but not by 20; otherwise e(k)=0.
    Thus each factor with e(k)=2 contributes
          (1 - 2 q^k + q^{2k}).

    The routine below multiplies these factors successively
    to obtain the coefficients up to any requested index.
*/

#include <stdlib.h>
#include <stdbool.h>

/*---  static storage for the computed coefficients  -------------------*/
static seq_t *coeff = NULL;      /* array of coefficients, coeff[idx] = coefficient of q^idx */
static idx_t max_computed = -1;  /* largest idx for which coeff has been computed */

/*---  recompute all coefficients up to index upto (inclusive)  --------*/
static void recompute(idx_t upto)
{
    size_t sz = (size_t)(upto + 1);
    seq_t *new_coeff = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!new_coeff) exit(1);      /* abort on allocation failure */

    /* initialise */
    for (size_t i = 0; i < sz; ++i) new_coeff[i] = 0;
    new_coeff[0] = 1;    /* empty product starts with 1 */

    /* multiply in the required factors */
    for (idx_t k = 1; k <= upto; ++k) {
        bool has_factor = ((k % 5 != 0 && k % 2 != 0) || (k % 4 == 0 && k % 20 != 0));
        if (!has_factor) continue;

        /* in-place convolution with (1 - 2q^k + q^{2k}) */
        for (idx_t i = upto; i >= 0; --i) {
            seq_t val = new_coeff[i];
            if (i >= k)    val -= 2 * new_coeff[i - k];
            if (i >= 2 * k) val += new_coeff[i - 2 * k];
            new_coeff[i] = val;
        }
    }

    /* replace old array */
    if (coeff) free(coeff);
    coeff = new_coeff;
    max_computed = upto;
}

/*---  the requested sequence function  ---------------------------------*/
seq_t a(idx_t n)
{
    /* indices smaller than the offset are undefined */
    if (n < -1) return 0;

    idx_t idx = n + 1;                /* coefficient of q^idx is a(n) */
    if (coeff == NULL || idx > max_computed)
        recompute(idx);

    return coeff[idx];
}
