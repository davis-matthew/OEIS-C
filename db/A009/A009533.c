/*
 * A009533  Expansion of sin(x)*cos(sin(x)).
 * https://oeis.org/A009533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (2n+1)! * [x^(2n+1)] sin(x) * cos(sin x)
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 7, i64 n <= 12, i128 n <= 15,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* The following utility functions are available in the build environment.
   We use only the binomial coefficient function. */
#include "../utils/binom.h"

#define MAX_N 10000
#define MAX_M (2*MAX_N + 1)

/* Arrays that store the integer coefficients derived from
   the power series of cos(sin x).  aCoeff[j] holds
   j! times the coefficient of x^j in cos(sin x).  */
static seq_t aCoeff[MAX_M + 1];
static seq_t bCoeff[MAX_M + 1]; /* j! times the coefficient of x^j in sin(sin x) */

static idx_t computedM = 0;   /* largest exponent for which aCoeff and bCoeff
                                 have been computed (0-based). */

/* Ensure that aCoeff[0..M_needed] and bCoeff[0..M_needed] are computed,
   where M_needed = 2*maxN + 1.  The recurrence for the scaled
   coefficients is:
        a[0] = 1
        b[0] = 0
        a[n+1] = - Σ_{i=0}^{n} b[i] * r[n-i] * C(n,i)
        b[n+1] =   Σ_{i=0}^{n} a[i] * r[n-i] * C(n,i)
   with r[2k] = (-1)^k, r[odd] = 0.  */
static void ensureSeries(idx_t maxN)
{
    idx_t M_needed = 2 * maxN + 1;
    if (computedM >= M_needed)
        return;

    /* initialise the base case if we are starting from scratch */
    if (computedM == 0) {
        aCoeff[0] = 1;
        bCoeff[0] = 0;
    }

    for (idx_t n = computedM; n < M_needed; ++n) {
        seq_t sumA = 0;
        seq_t sumB = 0;
        seq_t bin  = 1;               /* C(n,0) */
        for (idx_t i = 0; i <= n; ++i) {
            /* r[n-i] is non‑zero only when n-i is even */
            if (((n - i) & 1) == 0) {
                idx_t half = (n - i) >> 1;
                seq_t r_val = ((half & 1) ? -1 : 1);   /* (-1)^half */
                sumA += bCoeff[i] * r_val * bin;
                sumB += aCoeff[i] * r_val * bin;
            }
            if (i < n) {
                bin = bin * (seq_t)(n - i) / (i + 1); /* C(n,i+1) */
            }
        }
        aCoeff[n + 1] = -sumA;
        bCoeff[n + 1] =  sumB;
    }
    computedM = M_needed;
}

/* Public function: returns the n-th term of the OEIS sequence A009533. */
seq_t a(idx_t n)
{
    /* compute all necessary coefficients once */
    ensureSeries(n);

    idx_t n2 = 2 * n + 1;          /* we need coefficient of x^{2n+1} in the product */
    seq_t result = 0;
    seq_t bin     = 1;             /* C(n2,0) */
    for (idx_t i = 0; i <= n2; ++i) {
        if (i & 1) {               /* only odd i contribute (sin terms) */
            idx_t half = (i - 1) / 2;
            seq_t sCoeff = ((half & 1) ? -1 : 1);   /* (-1)^half */
            seq_t cCoeff = aCoeff[n2 - i];          /* coefficient from cos(sin x) */
            result += sCoeff * cCoeff * bin;
        }
        if (i < n2) {
            bin = bin * (seq_t)(n2 - i) / (i + 1);  /* update to C(n2,i+1) */
        }
    }
    return result;
}
