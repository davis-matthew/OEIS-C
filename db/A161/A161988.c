/*
 * A161988  Number of reduced words of length n in the Weyl group B_33.
 * https://oeis.org/A161988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = coefficient of x^n in the polynomial
 *                ∏_{k=1}^{33} (1 + x + x^2 + … + x^{2k-1}),
 *           i.e. the number of reduced words of length n in Weyl group B_33.
 * Computes all coefficients up to the maximal degree 33^2 = 1089 using
 * successive convolution with all‑ones polynomials.
 * Sequence is zero for n > 1089 (longest word length is 1089).
 */
#include <stdlib.h>

static idx_t const B_M = 33;
static idx_t const MAX_DEG = B_M * B_M;           /* 33^2 = 1089 */

static seq_t *coeff_arr = NULL;  /* array of coefficients 0..MAX_DEG */
static idx_t  coeff_deg = 0;     /* current maximum degree after computation */

/* one‑time initialization of coeff_arr */
static void init_coeff_array(void)
{
    if (coeff_arr != NULL)
        return;                    /* already initialized */

    coeff_arr = (seq_t *)malloc((MAX_DEG + 1) * sizeof(seq_t));
    if (coeff_arr == NULL) {
        /* Allocation failure; return zero on all queries */
        return;
    }
    for (idx_t i = 0; i <= MAX_DEG; ++i)
        coeff_arr[i] = 0;
    coeff_arr[0] = 1;              /* start polynomial = 1 */
    coeff_deg = 0;

    /* temporary array for intermediate convolution results */
    seq_t *tmp = (seq_t *)malloc((MAX_DEG + 1) * sizeof(seq_t));
    if (tmp == NULL) {
        free(coeff_arr);
        coeff_arr = NULL;
        return;
    }

    /* successive convolution with all‑ones polynomials of lengths 2,4,…,66 */
    for (idx_t k = 1; k <= B_M; ++k) {
        idx_t len    = 2 * k;           /* number of terms in factor (1 + … + x^{2k-1}) */
        idx_t newdeg = coeff_deg + len - 1;

        /* zero tmp[0..newdeg] */
        for (idx_t i = 0; i <= newdeg; ++i)
            tmp[i] = 0;

        /* add contributions */
        for (idx_t i = 0; i <= coeff_deg; ++i) {
            seq_t val = coeff_arr[i];
            for (idx_t j = 0; j < len; ++j)
                tmp[i + j] += val;
        }

        /* copy back to coeff_arr */
        for (idx_t i = 0; i <= newdeg; ++i)
            coeff_arr[i] = tmp[i];

        coeff_deg = newdeg;
    }

    free(tmp);
}

seq_t a(idx_t n)
{
    init_coeff_array();
    if (coeff_arr == NULL)
        return (seq_t)0;          /* allocation failed, fallback to 0 */

    if (n <= coeff_deg)
        return coeff_arr[n];
    else
        return (seq_t)0;          /* terms beyond the maximal degree vanish */
}
