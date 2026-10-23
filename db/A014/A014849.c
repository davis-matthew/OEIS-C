/*
 * A014849  Inverse of 840th cyclotomic polynomial.
 * https://oeis.org/A014849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = the coefficient of xⁿ in the formal power series
 *            1/Φ₈₄₀(x); the sequence is periodic with period 840
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   poly_div_exact
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <string.h>
#include "../utils/poly_div_exact.h"

/* multiply two polynomials A (degA) and B (degB).
   Result stored in Res (size at least degA+degB+1). */
static void poly_mul(const seq_t *A, int degA,
                     const seq_t *B, int degB,
                     seq_t *Res, int *DegRes)
{
    int size = degA + degB + 1;
    for (int i = 0; i < size; ++i) Res[i] = 0;
    int max = -1;
    for (int i = 0; i <= degA; ++i)
        for (int j = 0; j <= degB; ++j) {
            Res[i + j] += A[i] * B[j];
            if (Res[i + j] != 0 && i + j > max) max = i + j;
        }
    *DegRes = max;
}

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t a_cache[840];
    static seq_t phi_cache[842];      /* coefficients of Φ₈₄₀(x) */

    if (!initialized) {
        /* compute Φ₈₄₀(x) */
        int divs[64], ndiv = 0;
        for (int d = 1; d <= 840; ++d)
            if (840 % d == 0) divs[ndiv++] = d;

        /* store Φ_d for all divisors of 840 */
        seq_t poly[841][842];
        int degPoly[841];
        for (int i = 0; i < 841; ++i) degPoly[i] = -1;
        for (int i = 0; i < 841; ++i) memset(poly[i], 0, sizeof(poly[i]));

        for (int idx = 0; idx < ndiv; ++idx) {
            int d = divs[idx];
            if (d == 1) {
                poly[1][0] = -1;
                poly[1][1] = 1;
                degPoly[1] = 1;
            } else {
                seq_t prod[842], tmp[842];
                int degProd = 0;
                memset(prod, 0, sizeof(prod));
                prod[0] = 1;  /* start with polynomial 1 */
                for (int j = 0; j < idx; ++j) {
                    int c = divs[j];
                    if (d % c == 0) {
                        int degTmp;
                        poly_mul(prod, degProd, poly[c], degPoly[c], tmp, &degTmp);
                        for (int i = 0; i <= degTmp; ++i) prod[i] = tmp[i];
                        degProd = degTmp;
                    }
                }
                seq_t numer[842];
                memset(numer, 0, sizeof(numer));
                numer[0] = -1;
                numer[d] = 1;
                int degQ;
                poly_div_exact(numer, d, prod, degProd, tmp, &degQ);
                for (int i = 0; i <= degQ; ++i) poly[d][i] = tmp[i];
                degPoly[d] = degQ;
            }
        }

        int phi_deg = degPoly[840];
        for (int i = 0; i <= phi_deg; ++i) phi_cache[i] = poly[840][i];

        /* compute the periodic values a(0)…a(839) using the recurrence
           a(n) = –∑_{k≥1} Φ₈₄₀,ₖ · a(n–k)  (with Φ₈₄₀,₀ = 1). */
        a_cache[0] = 1;
        for (int m = 1; m < 840; ++m) {
            seq_t sum = 0;
            int maxk = (m < phi_deg) ? m : phi_deg;
            for (int k = 1; k <= maxk; ++k)
                sum += phi_cache[k] * a_cache[m - k];
            a_cache[m] = -sum;
        }

        initialized = true;
    }

    return a_cache[n % 840];
}
