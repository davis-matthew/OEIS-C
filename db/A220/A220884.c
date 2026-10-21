/*
 * A220884  Triangle read by rows: row n gives coefficients of expansion of
 *          Product_{k=2..n} ((n+1-k)*x+k), starting with lowest power.
 * https://oeis.org/A220884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 58, u64 n <= 142,
 *            u128 n <= 389, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence a(n) from OEIS A220884:
 * Triangle read by rows: the n‑th row (starting with n=0) contains the 
 * coefficients of the polynomial
 *    ∏_{k=2..n} ( (n+1-k)·x + k )
 *   starting with the constant term.  The sequence is obtained by 
 *   concatenating the rows.
 */
#include <stdlib.h>

/* Compute the coefficient at the given global index n. */
seq_t a(idx_t n)
{
    /* Find the row r and position pos within that row. */
    idx_t sum = 0;          /* total number of terms seen before current row */
    idx_t r   = 0;          /* current row index */
    for (;; ++r) {
        /* Length of the row.  Row 0 and 1 have length 1; otherwise r. */
        idx_t len = (r < 2) ? 1 : r;
        if (n < sum + len) {               /* n lies in this row   */
            idx_t pos = n - sum;           /* position in the row  */
            if (r <= 1)                     /* rows 0 and 1 are just [1] */
                return (seq_t)1;

            /* Allocate array for the polynomial coefficients.
             * The product has degree r-1, so we need r coefficients. */
            seq_t *coeff = (seq_t *)calloc((size_t)r, sizeof(seq_t));
            if (!coeff)                     /* allocation failure */
                return (seq_t)0;

            /* Start with the constant polynomial 1. */
            coeff[0] = (seq_t)1;
            size_t deg = 0;                /* current degree */

            /* Multiply successively by each factor ((r+1-k)·x + k). */
            for (idx_t k = 2; k <= r; ++k) {
                seq_t a_coef = (seq_t)(r + 1 - k);  /* coefficient of x  */
                seq_t b_coef = (seq_t)k;            /* constant term     */

                /* New highest degree term comes from the x‑coefficient. */
                coeff[deg + 1] = a_coef * coeff[deg];

                /* Update the remaining terms, iterating downwards to use
                 * the old coefficient values. */
                for (size_t i = deg; i > 0; --i) {
                    coeff[i] = a_coef * coeff[i - 1] + b_coef * coeff[i];
                }

                /* Update the constant term. */
                coeff[0] = b_coef * coeff[0];
                ++deg;
            }

            seq_t result = coeff[pos];
            free(coeff);
            return result;
        }
        sum += len;     /* skip this row and continue */
    }
}
