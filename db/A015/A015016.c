/*
 * A015016  Inverse of 1007th cyclotomic polynomial.
 * https://oeis.org/A015016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the coefficient of x^n in the power series of
 *            1/Φ₁₀₀₇(x), where Φ₁₀₀₇ is the 1007‑th cyclotomic polynomial.
 *            The sequence is periodic with period 1007
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t coeff[937];   /* coefficients of Φ₁₀₀₇(x), degree 936 */
static seq_t a_seq[1007];  /* first period of the reciprocal series */
static bool initialized = false;

/* build Φ₁₀₀₇(x) via long division of (x¹⁰⁰⁸−x¹⁰⁰⁷−x+1)/(x⁷²−x⁵³−x¹⁹+1) */
static void init_sequence(void)
{
    if (initialized) return;
    initialized = true;

    /* numerator : x^1008 - x^1007 - x + 1  (degree 1008) */
    seq_t num[1009] = {0};
    num[1008] = 1;
    num[1007] = -1;
    num[1]   = -1;
    num[0]   = 1;

    /* denominator : x^72 - x^53 - x^19 + 1  (degree 72) */
    seq_t den[73] = {0};
    den[72] = 1;
    den[53] = -1;
    den[19] = -1;
    den[0]  = 1;

    /* long division : num / den  -> Φ₁₀₀₇(x) */
    for (int i = 1008; i >= 72; --i) {
        seq_t factor = num[i];
        coeff[i - 72] = factor;
        if (factor != 0) {
            int shift = i - 72;
            for (int j = 0; j <= 72; ++j) {
                num[shift + j] -= factor * den[j];
            }
        }
    }

    /* a(x) = 1 / Φ₁₀₀₇(x)  =>  a(0)=1  and
         a(n)+∑_{k=1}^{936} coeff[k]·a(n-k)=0  (n≥1)  */
    a_seq[0] = 1;
    for (idx_t n = 1; n < 1007; ++n) {
        seq_t sum = 0;
        idx_t maxk = n < 936 ? n : 936;
        for (idx_t k = 1; k <= maxk; ++k) {
            sum += coeff[k] * a_seq[n - k];
        }
        a_seq[n] = -sum;
    }
}

seq_t a(idx_t n)
{
    if (!initialized) init_sequence();
    return a_seq[(size_t)(n % 1007)];
}
