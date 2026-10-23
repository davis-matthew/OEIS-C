/*
 * A162359  Number of reduced words of length n in the Weyl group D_20.
 * https://oeis.org/A162359
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of x^n in * (1 - x^20)/(1 - x) * ∏_{i=1}^{19}
 *            (1 - x^{2i})/(1 - x) * i.e. a(n) is the coefficient of x^n in
 *            the product * f(20) * ∏_{i=1}^{19} f(2i), * where f(m) = (1 -
 *            x^m)/(1 - x) = 1 + x + … + x^{m-1}
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 16, u64 n <= 85, u128 n <= 380,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int init_flag = 0;             /* 0: not computed, 1: computed */
static seq_t coeff[381];              /* coefficients of the product polynomial */

/* Pre‑compute the coefficients once and store them in coeff[]. */
static void precompute(void)
{
    if (init_flag) return;

    /* initialise to zeros */
    for (idx_t i = 0; i < 381; ++i) coeff[i] = 0;
    coeff[0] = 1;
    idx_t cur_deg = 0;                     /* current maximum degree */
    seq_t temp[381];                       /* temporary array for convolution */

    /* multiply by f(20) = 1 + x + … + x^19 */
    {
        idx_t m = 20;
        idx_t new_deg = cur_deg + m - 1;
        for (idx_t i = 0; i <= new_deg; ++i) temp[i] = 0;
        for (idx_t i = 0; i <= cur_deg; ++i) {
            seq_t val = coeff[i];
            if (val == 0) continue;
            for (idx_t j = 0; j < m; ++j) {
                temp[i + j] += val;
            }
        }
        for (idx_t i = 0; i <= new_deg; ++i) coeff[i] = temp[i];
        cur_deg = new_deg;
    }

    /* multiply by f(2), f(4), …, f(38) */
    for (idx_t even = 2; even <= 38; even += 2) {
        idx_t m = even;
        idx_t new_deg = cur_deg + m - 1;
        for (idx_t i = 0; i <= new_deg; ++i) temp[i] = 0;
        for (idx_t i = 0; i <= cur_deg; ++i) {
            seq_t val = coeff[i];
            if (val == 0) continue;
            for (idx_t j = 0; j < m; ++j) {
                temp[i + j] += val;
            }
        }
        for (idx_t i = 0; i <= new_deg; ++i) coeff[i] = temp[i];
        cur_deg = new_deg;
    }

    init_flag = 1;
}

/* Return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    precompute();
    if (n <= 380) return coeff[n];
    /* beyond degree 380 all coefficients vanish */
    seq_t zero = 0;
    return zero;
}
