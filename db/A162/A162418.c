/*
 * A162418  Number of reduced words of length n in the Weyl group D_45.
 * https://oeis.org/A162418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{e∈{1,3,…,87,44}} (1 + x + … +
 *            x^e)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is the number of reduced words of length n in the Weyl group D_45.
   The implementation precomputes the coefficients of the
   Poincaré polynomial using a dynamic‑programming approach that
   adds the polynomials for each exponent. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Forward declaration of the sequence function */
seq_t a(idx_t n);

/* Pre‑computed coefficients of the Poincaré polynomial.
   The maximum degree is sum of all exponents:
   1 + 3 + … + 87 + 44 = 1980. */
#define MAX_DEG 1980

static seq_t *coeff = NULL;
static bool coeff_init = false;

/* Helper to precompute the coefficient array. */
static void init_coeff(void)
{
    /* Allocate space for all coefficients. */
    coeff = (seq_t*)calloc(MAX_DEG + 1, sizeof(seq_t));
    if (!coeff) return;          /* Allocation failure is ignored – build system will not use this path. */
    coeff[0] = (seq_t)1;         /* Empty product gives coefficient 1 for degree 0. */

    /* List of exponents for D_45:
       odd numbers 1,3,…,87 (44 terms) and the extra exponent 44. */
    int exps[45];
    size_t exps_cnt = 0;
    for (int i = 1; i <= 44; ++i) {
        exps[exps_cnt++] = 2*i - 1;    /* 1,3,5,…,87 */
    }
    exps[exps_cnt++] = 44;            /* m-1 for D_m (m=45) */

    size_t current_deg = 0;
    for (size_t ei = 0; ei < exps_cnt; ++ei) {
        int e = exps[ei];
        /* Update coefficients in place.
           Process indices in decreasing order to avoid using values
           that were just updated during this step. */
        for (size_t i = current_deg; ; --i) {
            seq_t val = coeff[i];
            if (val != (seq_t)0) {
                for (int k = 1; k <= e; ++k) {
                    coeff[i + k] += val;
                }
            }
            if (i == 0) break;
        }
        current_deg += (size_t)e;
    }

    coeff_init = true;
}

seq_t a(idx_t n)
{
    if (!coeff_init) {
        init_coeff();
        if (!coeff_init) return (seq_t)0;  /* Out of memory – return zero. */
    }
    if (n > (idx_t)MAX_DEG) return (seq_t)0;
    return coeff[(size_t)n];
}
