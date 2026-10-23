/*
 * A016399  391st cyclotomic polynomial.
 * https://oeis.org/A016399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   not stated
 * Range:     i8 n <= 352, i16 n <= 352, i32 n <= 352, i64 n <= 352,
 *            i128 n <= 352, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 391st cyclotomic polynomial.
    Φ_391(x) = (x-1)(x^391-1) / [(x^17-1)(x^23-1)]
    The coefficients a(n) for n=0..352 are returned.  */
/*  Expected size of Φ_391 is φ(391)=352.  */
#include <stddef.h>
#include <stdbool.h>

static seq_t coeff[353];
static bool init = false;

static void compute_phi_391(void)
{
    /* 392 = deg((x-1)(x^391-1)), 40 = deg((x^17-1)(x^23-1)) */
    static seq_t num[393] = {0};
    static seq_t den[41]  = {0};
    const size_t degN = 392, degD = 40, degQ = 352;

    /* numerator: (x-1)(x^391-1) = x^392 - x - x^391 + 1 */
    num[0]   =  1;          /* constant term */
    num[1]   = -1;          /* -x */
    num[391] = -1;          /* -x^391 */
    num[392] =  1;          /* x^392 */

    /* denominator: (x^17-1)(x^23-1) = x^40 - x^17 - x^23 + 1 */
    den[0]   =  1;          /* constant term */
    den[17]  = -1;          /* -x^17 */
    den[23]  = -1;          /* -x^23 */
    den[40]  =  1;          /* x^40 */

    /* initialise quotient array */
    for (size_t i = 0; i <= degQ; ++i) coeff[i] = 0;

    /* Perform exact polynomial long division N/D = Q */
    for (int i = (int)degN; i >= (int)degD; --i) {
        const seq_t factor = num[i];          /* coefficient to remove */
        const size_t qpos  = (size_t)(i - degD); /* position in quotient */

        coeff[qpos] = factor;                 /* set quotient coefficient */

        /* subtract factor * D(x) shifted by qpos from numerator */
        for (size_t j = 0; j <= degD; ++j) {
            num[qpos + j] -= factor * den[j];
        }
    }
}

seq_t a(idx_t n)
{
    if (!init) {
        compute_phi_391();
        init = true;
    }
    if (n >= 353)                /* outside the known degree */
        return 0;
    return coeff[n];
}
