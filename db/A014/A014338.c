/*
 * A014338  Inverse of 329th cyclotomic polynomial.
 * https://oeis.org/A014338
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

/* Sequence A014338: Inverse of the 329th cyclotomic polynomial.   
   It satisfies the recurrence
   a(0) = 1,
   a(n) = -∑_{k=1}^{φ(329)} c_k · a(n-k)  for n ≥ 1,
   where Φ₃₂₉(x) = ∑_{k=0}^{φ(329)} c_k x^k.  
   The sequence is periodic with period 329. */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t coeffs[277];   /* coefficients of Φ₃₂₉(x), degree 276 */
static seq_t values[329];   /* a(n) for 0 ≤ n < 329, periodic after that */
static bool initialized = false;

/* Compute the coefficients of Φ₃₂₉(x) by dividing (x³³⁰‑x²⁸⁹‑x+1)
   by (x⁵⁴‑x⁴⁷‑x⁷+1). */
static void init_coeffs(void)
{
    seq_t num[331]   = {0};  /* numerator polynomial, deg 330 */
    seq_t den[55]    = {0};  /* denominator polynomial, deg 54 */
    seq_t rem[331]   = {0};  /* remainder during division */
    seq_t quo[277]   = {0};  /* quotient polynomial, coeffs 0..276 */

    /* numerator: x³³⁰ − x²⁸⁹ − x + 1 */
    num[0]   = (seq_t)1;           /* constant term */
    num[1]   = (seq_t)-1;          /* −x */
    num[329] = (seq_t)-1;          /* −x²⁸⁹ */
    num[330] = (seq_t)1;           /* x³³⁰ */

    /* copy to remainder buffer */
    for (size_t i = 0; i < 331; ++i) rem[i] = num[i];

    /* denominator: x⁵⁴ − x⁴⁷ − x⁷ + 1 */
    den[0]  = (seq_t)1;
    den[7]  = (seq_t)-1;
    den[47] = (seq_t)-1;
    den[54] = (seq_t)1;

    /* long division: denominator leading coeff = 1 */
    for (long long i = 330; i >= 54; --i) {
        seq_t coeff = rem[i];
        if (coeff != (seq_t)0) {
            size_t qidx = (size_t)(i - 54);
            quo[qidx] = coeff;
            /* subtract coeff * den from remainder */
            for (size_t j = 0; j <= 54; ++j) {
                rem[i - 54 + j] -= coeff * den[j];
            }
        }
    }

    /* store quotient coefficients */
    for (size_t k = 0; k <= 276; ++k) coeffs[k] = quo[k];
}

/* Compute a(n) for 0 ≤ n < 329 using the recurrence. */
static void init_values(void)
{
    values[0] = (seq_t)1; /* a(0) = 1 */
    for (idx_t n = 1; n < 329; ++n) {
        seq_t sum = 0;
        size_t maxk = (n < 277 ? (size_t)n : 276);
        for (size_t k = 1; k <= maxk; ++k) {
            sum += coeffs[k] * values[n - k];
        }
        values[n] = -sum;
    }
}

seq_t a(idx_t n)
{
    if (!initialized) {
        init_coeffs();
        init_values();
        initialized = true;
    }
    return values[n % 329ULL];
}
