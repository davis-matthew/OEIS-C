/*
 * A014057  Inverse of 48th cyclotomic polynomial.
 * https://oeis.org/A014057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) is the coefficient of x^n in the power series 1/Φ48(x),
 *  where Φ48 is the 48th cyclotomic polynomial.
 *  The sequence satisfies a(0)=1 and for n>0
 *
 *      a(n) = - Σ_{k=1..16} Φ48_coeff[k] * a(n-k),
 *
 *  with Φ48_coeff[0]=1, Φ48_coeff[1..16] the coefficients of Φ48(x)
 *  (constant term first, leading coefficient last).
 *  The sequence is periodic with period 48.
 */

#include <stdint.h>

/* Period of the sequence */
#define PERIOD 48U
/* Degree of Φ48(x) */
#define DEGR 16U
/* Sum of degrees of the proper divisors of 48 */
#define DEG_PROD 32U
/* Number of proper cyclotomic factors of Φ48(x) */
#define NPOLY 9U

/* Move the cyclotomic coefficients into read‑only static storage */
static const seq_t poly1[9]  = { -1,  1, 0,0,0,0,0,0,0 }; /* Φ1   x-1   */
static const seq_t poly2[9]  = {  1,  1, 0,0,0,0,0,0,0 }; /* Φ2   x+1   */
static const seq_t poly3[9]  = {  1,  1,  1,0,0,0,0,0,0 }; /* Φ3   x²+x+1 */
static const seq_t poly4[9]  = {  1,  0,  1,0,0,0,0,0,0 }; /* Φ4   x²+1   */
static const seq_t poly6[9]  = {  1, -1,  1,0,0,0,0,0,0 }; /* Φ6   x²-x+1 */
static const seq_t poly8[9]  = {  1,  0,  0, 0, 1,0,0,0,0 }; /* Φ8   x⁴+1   */
static const seq_t poly12[9] = {  1,  0, -1, 0, 1,0,0,0,0 }; /* Φ12  x⁴-x²+1*/
static const seq_t poly16[9] = {  1,  0,  0, 0, 0,0,0,0, 1 }; /* Φ16  x⁸+1   */
static const seq_t poly24[9] = {  1,  0,  0, 0,-1,0,0,0, 1 }; /* Φ24  x⁸-x⁴+1*/

/* Helper structure to iterate over the proper factors */
struct poly { const seq_t *c; idx_t d; } ;
static const struct poly polys[NPOLY] = {
    { poly1,  1U },
    { poly2,  1U },
    { poly3,  2U },
    { poly4,  2U },
    { poly6,  2U },
    { poly8,  4U },
    { poly12, 4U },
    { poly16, 8U },
    { poly24, 8U }
};

/* Computes a(n) for all n ≥ 0 */
seq_t a(idx_t n)
{
    /* ------------------------------------------------------------
     *  1.  Create a static cache of the period coefficients
     * ------------------------------------------------------------ */
    static seq_t period[PERIOD];
    static int initialized = 0;

    if (!initialized)
    {
        /* 1.1  Compute product of all proper factors  */
        seq_t prod[DEG_PROD + 1];
        seq_t temp[DEG_PROD + 1];
        idx_t cur_deg = 0;

        /* zero product array */
        for (idx_t i = 0; i <= DEG_PROD; ++i) prod[i] = 0;
        prod[0] = 1; /* start with 1 */

        for (idx_t p = 0; p < NPOLY; ++p)
        {
            const seq_t *P = polys[p].c;
            idx_t degP = polys[p].d;

            /* zero temporary buffer up to new degree */
            for (idx_t i = 0; i <= cur_deg + degP; ++i) temp[i] = 0;

            /* convolution prod * P */
            for (idx_t i = 0; i <= cur_deg; ++i)
                for (idx_t j = 0; j <= degP; ++j)
                    temp[i + j] += prod[i] * P[j];

            /* copy back the new product */
            cur_deg += degP;
            for (idx_t i = 0; i <= cur_deg; ++i) prod[i] = temp[i];
        }

        /* 1.2  Divide (x^48 - 1) by the product to obtain Φ48 */
        seq_t Q[49];
        seq_t R[DEGR + 1];

        for (idx_t i = 0; i <= 48; ++i) Q[i] = 0;
        Q[0] = -1;      /* constant term */
        Q[48] = 1;      /* x^48 term */

        for (idx_t i = 0; i <= DEGR; ++i) R[i] = 0;

        /* long division: Q = Prod * R + rem (rem becomes zero) */
        for (idx_t i = 48; i >= DEG_PROD; --i)
        {
            seq_t coeff = Q[i];
            idx_t shift = i - DEG_PROD;
            R[shift] = coeff;

            /* subtract coeff * x^shift * prod from Q */
            for (idx_t j = 0; j <= DEG_PROD; ++j)
                Q[shift + j] -= coeff * prod[j];
        }

        /* 1.3  Compute the inverse series coefficients for one period */
        seq_t a0[PERIOD];
        for (idx_t i = 0; i < PERIOD; ++i) a0[i] = 0;
        a0[0] = 1; /* a(0) = 1 */

        for (idx_t nn = 1; nn < PERIOD; ++nn)
        {
            seq_t sum = 0;
            idx_t maxk = (nn < DEGR) ? nn : DEGR;
            for (idx_t k = 1; k <= maxk; ++k)
                sum += R[k] * a0[nn - k];
            a0[nn] = -sum;
        }

        /* Store the period in the static cache */
        for (idx_t i = 0; i < PERIOD; ++i)
            period[i] = a0[i];

        initialized = 1;
    }

    /* ------------------------------------------------------------
     *  2. Return the requested element
     * ------------------------------------------------------------ */
    return period[n % PERIOD];
}
