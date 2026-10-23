/*
 * A015668  Inverse of 1659th cyclotomic polynomial.
 * https://oeis.org/A015668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   mul_by_x_minus_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The sequence a(n) is given by the power‑series inverse of the 1659‑th
    cyclotomic polynomial Φ₁₆₅₉(x).  That is, if

          Φ₁₆₅₉(x) = 1 + c₁x + c₂x² + … + c₉₃₆x⁹³⁶,

    then 1/Φ₁₆₅₉(x) = Σ a(n) xⁿ   and a(0)=1.  The coefficients a(n) are
    periodic with period 1659.  The recurrence

          a(0)=1,   a(n) = –∑₁ⁱ≤deg Φ₁₆₅₉ cᵢ·a(n–i)   (n≥1)

    is used after the coefficients cᵢ are computed from the cyclotomic
    polynomial. */

#include <stdlib.h>
#include "../utils/mul_by_x_minus_one.h"

/* ---------- constants --------------------------------------------------------------------- */
static const idx_t PERIOD   = 1659;
static const idx_t POS_DEG[4] = { 3,  7, 79, 1659 };   /* exponents of (x^d‑1) with μ(n/d)=+1 */
static const idx_t NEG_DEG[4] = { 1, 21,237, 553 };    /* exponents of (x^d‑1) with μ(n/d)=‑1 */

/* ---------- global storage --------------------------------------------------------------- */
static seq_t *periodic = NULL;

/* ---------- initialise the sequence ------------------------------------------------------ */
static void init_sequence(void)
{
    if (periodic != NULL)
        return;   /* already initialised */

    /* 1.  build numerator N(x) = Π_{μ(n/d)=+1} (x^d‑1) */
    const idx_t sum_pos = POS_DEG[0] + POS_DEG[1] + POS_DEG[2] + POS_DEG[3];   /* 1748 */
    seq_t *num = (seq_t*)calloc(sum_pos + 1, sizeof(seq_t));
    if (!num) abort();
    num[0] = 1;
    idx_t degN = 0;
    for (size_t k = 0; k < 4; ++k)
        mul_by_x_minus_one(num, &degN, POS_DEG[k]);

    /* 2.  build denominator D(x) = Π_{μ(n/d)=‑1} (x^d‑1) */
    const idx_t sum_neg = NEG_DEG[0] + NEG_DEG[1] + NEG_DEG[2] + NEG_DEG[3];   /* 812 */
    seq_t *den = (seq_t*)calloc(sum_neg + 1, sizeof(seq_t));
    if (!den) abort();
    den[0] = 1;
    idx_t degD = 0;
    for (size_t k = 0; k < 4; ++k)
        mul_by_x_minus_one(den, &degD, NEG_DEG[k]);

    /* 3.  divide N by D to obtain Φ₁₆₅₉(x)  (exact division) */
    idx_t degQ = degN - degD;          /* φ(1659) = 936 */
    seq_t *coeff = (seq_t*)malloc((degQ + 1) * sizeof(seq_t));
    if (!coeff) abort();

    for (idx_t i = degN; i >= degD; --i) {
        seq_t q = num[i];
        coeff[i - degD] = q;
        for (idx_t j = 0; j <= degD; ++j)
            num[i - degD + j] -= q * den[j];
    }

    free(num);
    free(den);

    /* 4.  compute the periodic coefficients a(n) for n=0…PERIOD‑1  */
    periodic = (seq_t*)calloc(PERIOD, sizeof(seq_t));
    if (!periodic) abort();

    periodic[0] = 1;
    for (idx_t n = 1; n < PERIOD; ++n) {
        seq_t sum = 0;
        idx_t limit = (n <= degQ) ? n : degQ;
        for (idx_t i = 1; i <= limit; ++i)
            sum += coeff[i] * periodic[n - i];
        periodic[n] = -sum;
    }

    free(coeff);
}

/* ---------- public function --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    init_sequence();
    return periodic[n % PERIOD];
}
