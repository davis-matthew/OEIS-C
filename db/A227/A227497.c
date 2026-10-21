/*
 * A227497  The hyper-Wiener index of the nanostar dendrimer defined
 *          pictorially as NS_3[n] in the Ashrafi et al. references.
 * https://oeis.org/A227497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2539 + 2^n*(-79425 + 368*n - 416*n^2) + 4^n*(113822 +
 *            15808*n + 43264*n^2)
 * Range:     u8 none, u16 none, u32 n <= 5, u64 n <= 19, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Multiply a small base (2 or 4) by itself exp times.
 * The result is stored in a local variable of type seq_t.  */
static seq_t small_pow(seq_t base, idx_t exp)
{
    seq_t r = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        r *= base;
    return r;
}

seq_t a(idx_t n)
{
    /* Intermediate coefficients as signed 64‑bit values to avoid
       intermediate overflow.  */
    int64_t n_i64 = (int64_t)n;
    int64_t coeff1_signed = -79425 + 368 * n_i64 - 416 * n_i64 * n_i64;
    int64_t coeff1_abs    = -coeff1_signed;          /* always positive */
    int64_t coeff2_signed = 113822 + 15808 * n_i64 + 43264 * n_i64 * n_i64;

    /* Powers of the small bases.  */
    seq_t pow2n = small_pow((seq_t)2, n);
    seq_t pow4n = small_pow((seq_t)4, n);

    /* Contributions of the two polynomial terms.  */
    seq_t term1 = pow2n * (seq_t)coeff1_abs;          /* 2^n * |coeff1|    */
    seq_t term2 = pow4n * (seq_t)coeff2_signed;      /* 4^n * coeff2       */

    /* Final result.  term2 is guaranteed to be ≥ term1 for all n ≥ 1. */
    seq_t result = (seq_t)2539 + term2 - term1;
    return result;
}
