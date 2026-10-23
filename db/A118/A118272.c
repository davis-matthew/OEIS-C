/*
 * A118272  Expansion of q^(-2/3) * (eta(q) * eta(q^3) * eta(q^6) /
 *          eta(q^2))^2 in powers of q.
 * https://oeis.org/A118272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥1} (1 - x^k)^2 (1 - x^{3k})^2
 *            (1 + x^{2k} + x^{4k})^2 Offset: 0 (a(0) is the first term)
 * Range:     i8 n <= 75, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *global_coeff = NULL;
static seq_t *global_temp  = NULL;
static size_t coeff_len = 0;

/* Exponent offsets and coefficients for each factor */
static const int coeff_e1[3] = {0, 1, 2};
static const int coeff_c1[3] = {1, -2, 1};
static const int coeff_e2[3] = {0, 3, 6};
static const int coeff_c2[3] = {1, -2, 1};
static const int coeff_e3[5] = {0, 2, 4, 6, 8};
static const int coeff_c3[5] = {1, 2, 3, 2, 1};   /* (1 + q^{2k} + q^{4k})^2 */

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    if (N >= coeff_len) {
        /* Allocate larger working arrays */
        size_t new_len = N + 1;
        seq_t *coeff_new = (seq_t *)malloc(new_len * sizeof(seq_t));
        seq_t *temp_new  = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!coeff_new || !temp_new) abort();

        memset(coeff_new, 0, new_len * sizeof(seq_t));
        coeff_new[0] = 1;                /* start with the constant term */

        seq_t *curr = coeff_new;         /* current product */
        seq_t *next = temp_new;          /* buffer for the next product */

        for (size_t k = 1; k <= N; ++k) {
            /* clear next buffer */
            memset(next, 0, new_len * sizeof(seq_t));

            /* Multiply by the polynomial for this k */
            for (int ia = 0; ia < 3; ++ia) {
                size_t e1 = coeff_e1[ia] * k;
                seq_t  c1 = (seq_t)coeff_c1[ia];
                for (int ib = 0; ib < 3; ++ib) {
                    size_t e2 = coeff_e2[ib] * k;
                    seq_t  c2 = (seq_t)coeff_c2[ib];
                    for (int ic = 0; ic < 5; ++ic) {
                        size_t e3 = coeff_e3[ic] * k;
                        seq_t  c3 = (seq_t)coeff_c3[ic];

                        size_t e  = e1 + e2 + e3;
                        if (e > N) continue;          /* term too large */

                        seq_t coeff_ec = c1 * c2 * c3;

                        /* Convolve: shift by e and add */
                        size_t limit = N - e;
                        for (size_t i = 0; i <= limit; ++i) {
                            next[i + e] += curr[i] * coeff_ec;
                        }
                    }
                }
            }

            /* Swap for next iteration */
            seq_t *swap = curr;
            curr = next;
            next = swap;
        }

        /* Free previous arrays */
        if (global_coeff) free(global_coeff);
        if (global_temp)  free(global_temp);

        /* Store new arrays */
        global_coeff = curr;
        global_temp  = next;
        coeff_len    = new_len;
    }

    return global_coeff[N];
}
