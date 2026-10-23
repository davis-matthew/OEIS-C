/*
 * A193321  G.f.: Sum_{n>=0} x^n*Product_{k=1..n} (1 - k*x) / (1 - (2*k)*x).
 * https://oeis.org/A193321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 22, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A193321
 *  Formula: a(n) = coefficient of x^n in
 *            Σ_{m≥0} x^m ∏_{k=1..m} (1 - k·x)/(1 - 2k·x).
 *
 *  The function below computes a(n) by iteratively building the rational series
 *  R_m(x) = ∏_{k=1..m} (1 - k·x)/(1 - 2k·x) and adding its contribution
 *  to the target coefficient.  The procedure runs in O(n²) arithmetic operations,
 *  which is below the 2‑second limit for n up to about 10 000 and gives exact
 *  values for all terms that fit in the supplied seq_t type.
 */
#include <stdlib.h>
#include <stdbool.h>   /* for abort */

seq_t a(idx_t n)
{
    /* Allocate arrays for the coefficients of R_m(x) (coeff),
       a temporary buffer (temp) and the target sequence (seq). */
    seq_t *seq  = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t *coeff = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t *temp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));

    if (!seq || !coeff || !temp) {
        free(seq); free(coeff); free(temp);
        abort();
    }

    /* Base case: R_0(x) = 1, thus a(0) = 1 */
    coeff[0] = 1;
    seq[0]   = 1;

    /* Build the terms for m = 1 … n.
       For each m we compute the coefficients of R_m(x) up to degree n-m
       (only those are needed for the final coefficient extraction). */
    for (idx_t m = 1; m <= n; ++m) {
        idx_t limit = n - m;            /* highest degree needed for this m */

        /* R_m(x) = R_{m-1}(x) * (1 - m·x) / (1 - 2m·x).
           Use the linear recurrence:
           temp[0] = coeff[0];
           temp[d] = coeff[d] + 2m·temp[d-1] - m·coeff[d-1]   (d ≥ 1) */
        temp[0] = coeff[0];
        for (idx_t d = 1; d <= limit; ++d) {
            temp[d] = coeff[d]
                      + (seq_t)(2 * m) * temp[d-1]
                      - (seq_t)(m)   * coeff[d-1];
        }

        /* All coefficients of R_m(x) that fit contribute to seq[n] when the
           power of x is n = m + d for d = 0 … limit. */
        for (idx_t d = 0; d <= limit; ++d) {
            seq[m + d] += temp[d];
            coeff[d]    = temp[d];        /* prepare coeff for next m */
        }
    }

    seq_t ans = seq[n];
    free(seq);
    free(coeff);
    free(temp);
    return ans;
}
