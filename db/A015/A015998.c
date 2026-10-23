/*
 * A015998  Inverse of 1989th cyclotomic polynomial.
 * https://oeis.org/A015998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, changed
 * Formula:   a(n) = coefficient of x^n in the power‑series expansion of
 *            1/Φ_{1989}(x); the sequence is periodic with period 1989
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include <stdbool.h>  /* for bool */

/* Static data used for one‑time initialization. */
static bool _a015998_inited = false;
static seq_t _a015998_coeff[1153];   /* Φ_{1989}(x) coefficients, 0..1152 */
static seq_t _a015998_pattern[1989]; /* first 1989 terms of the sequence */

/* --------------------------------------------------------------------- */
/* Helper: build Φ_{1989}(x) by product of (x^d-1) polynomials and then
   divide by the complementary product.  The result is the coefficient
   array of degree 1152. */
static void _a015998_compute(void)
{
    /* divisors of 1989 with μ(n/d)=+1 and -1 respectively   */
    /* μ(n/d)=+1 : 9, 39, 51, 1989  (product for numerator) */
    /* μ(n/d)=-1 : 3, 117, 153, 663 (product for denominator) */
    const idx_t num_divs[4] = {9, 39, 51, 1989};
    const idx_t den_divs[4] = {3, 117, 153, 663};
    const size_t MAX_NUM_DEG = 2100;  /* > (9+39+51+1989)=2088   */
    const size_t MAX_DEN_DEG = 950;   /* > (3+117+153+663)=936   */
    const size_t Q_DEG = 1153;        /* 1152+1                     */

    seq_t num[MAX_NUM_DEG];
    seq_t den[MAX_DEN_DEG];
    size_t degNum = 0, degDen = 0;

    /* ---------- Build numerator: (x^9-1)(x^39-1)(x^51-1)(x^1989-1) ---------- */
    for (size_t i = 0; i < MAX_NUM_DEG; ++i) num[i] = 0;
    num[0] = 1;
    for (size_t f = 0; f < 4; ++f) {
        size_t d = (size_t)num_divs[f];
        size_t newDeg = degNum + d;
        seq_t tmp[MAX_NUM_DEG];
        for (size_t i = 0; i <= newDeg; ++i) tmp[i] = 0;
        for (size_t i = 0; i <= degNum; ++i) {
            tmp[i] += -num[i];
            tmp[i + d] += num[i];
        }
        degNum = newDeg;
        for (size_t i = 0; i <= newDeg; ++i) num[i] = tmp[i];
    }

    /* ---------- Build denominator: (x^3-1)(x^117-1)(x^153-1)(x^663-1) ---------- */
    for (size_t i = 0; i < MAX_DEN_DEG; ++i) den[i] = 0;
    den[0] = 1;
    for (size_t f = 0; f < 4; ++f) {
        size_t d = (size_t)den_divs[f];
        size_t newDeg = degDen + d;
        seq_t tmp[MAX_DEN_DEG];
        for (size_t i = 0; i <= newDeg; ++i) tmp[i] = 0;
        for (size_t i = 0; i <= degDen; ++i) {
            tmp[i] += -den[i];
            tmp[i + d] += den[i];
        }
        degDen = newDeg;
        for (size_t i = 0; i <= newDeg; ++i) den[i] = tmp[i];
    }

    /* ---------- Divide numerator by denominator to obtain Φ_{1989}(x) ---------- */
    seq_t rem[MAX_NUM_DEG];  /* working copy of numerator */
    seq_t quot[Q_DEG];       /* quotient array  (degree 1152) */
    for (size_t i = 0; i <= degNum; ++i) rem[i] = num[i];

    /* Perform long division; denom is monic (leading coefficient 1). */
    for (long long i = (long long)degNum; i >= (long long)degDen; --i) {
        seq_t coeff = rem[(size_t)i];
        quot[(size_t)(i - (long long)degDen)] = coeff;
        for (size_t j = 0; j <= degDen; ++j) {
            rem[(size_t)(i - (long long)degDen + j)] -= coeff * den[j];
        }
    }

    /* Store Φ_{1989}(x) coefficients: Φ(x) = 1 + Σ_{i≥1} coeff[i] x^i */
    _a015998_coeff[0] = 1;
    for (size_t i = 1; i <= degNum - degDen; ++i) {
        _a015998_coeff[i] = quot[i];
    }

    /* ---------- Compute the first 1989 terms of the inverse series ---------- */
    const size_t PERIOD = 1989;
    const size_t DEG = degNum - degDen; /* 1152 */
    _a015998_pattern[0] = 1;
    for (idx_t k = 1; k < PERIOD; ++k) {
        seq_t sum = 0;
        size_t limit = k <= DEG ? (size_t)k : DEG;
        for (size_t i = 1; i <= limit; ++i) {
            sum += _a015998_coeff[i] * _a015998_pattern[(size_t)(k - i)];
        }
        _a015998_pattern[(size_t)k] = -sum;
    }

    _a015998_inited = true;
}

/* --------------------------------------------------------------------- */
/* Public API required by the framework.  Returns a(n). */
seq_t a(idx_t n)
{
    if (!_a015998_inited) _a015998_compute();
    return _a015998_pattern[(size_t)(n % 1989)];
}
