/*
 * A011650  105th cyclotomic polynomial.
 * https://oeis.org/A011650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of x^n in the 105th cyclotomic polynomial
 *            Φ₁₀₅(x)
 * Range:     i8 n <= 48, i16 n <= 48, i32 n <= 48, i64 n <= 48, i128 n <= 48,
 *            big any n
 * Validated: full, pass, 49 terms through n = 48 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* Generic polynomial division:  P = Q * div + rem
   Both P and div are monic (leading coefficient 1).  The quotient
   is written to Quotient[0 .. degP - degDiv] and its degree to *degQ_out.
   All intermediate values stay within the range of seq_t. */
static void divide_polynomial(
    const seq_t *P,      size_t degP,
    const seq_t *Div,    size_t degDiv,
    seq_t *Quotient,     size_t *degQ_out)
{
    seq_t R[121];
    for (size_t i = 0; i <= degP; ++i)
        R[i] = P[i];
    size_t rdeg = degP;
    size_t qdeg_max = degP - degDiv;      /* maximum possible degree of quotient */
    for (size_t i = 0; i <= qdeg_max; ++i)
        Quotient[i] = 0;

    while (rdeg >= degDiv) {
        seq_t coeff = R[rdeg];
        size_t shift = rdeg - degDiv;
        Quotient[shift] = coeff;
        for (size_t j = 0; j <= degDiv; ++j) {
            R[shift + j] -= coeff * Div[j];
        }
        while (rdeg > 0 && R[rdeg] == 0)
            --rdeg;
    }
    *degQ_out = qdeg_max;
}

static void mul_xd_minus_1(seq_t *poly, size_t *deg, idx_t d)
{
    size_t old_deg = *deg;
    size_t new_deg = old_deg + d;
    seq_t temp[121];                       /* 3+5+7+105 = 120 is the largest degree to appear */
    for (size_t i = 0; i <= new_deg; ++i) {
        seq_t val = 0;
        if (i <= old_deg)                 /* subtract poly[i]   */
            val -= poly[i];
        if (i >= d && (i - d) <= old_deg) /* add poly[i-d]     */
            val += poly[i - d];
        temp[i] = val;
    }
    for (size_t i = 0; i <= new_deg; ++i)
        poly[i] = temp[i];
    *deg = new_deg;
}

seq_t a(idx_t n)
{
    /* The coefficients of Φ₁₀₅(x) are non‑zero only up to degree 48. */
    if (n > 48)
        return 0;

    static bool init = false;
    static seq_t coeff[49];  /* coefficients a(0)..a(48) */

    if (!init) {
        /* 105 = 3 * 5 * 7  */
        static const idx_t pos_divs[4] = {3, 5, 7, 105};
        static const idx_t neg_divs[4] = {1, 15, 21, 35};

        /* Build N(x) = ∏_(d∈pos_divs) (x^d – 1) */
        seq_t N[121] = {0};
        size_t degN = 0;
        N[0] = 1;
        for (size_t k = 0; k < 4; ++k)
            mul_xd_minus_1(N, &degN, pos_divs[k]);

        /* Build D(x) = ∏_(d∈neg_divs) (x^d – 1) */
        seq_t D[73] = {0};
        size_t degD = 0;
        D[0] = 1;
        for (size_t k = 0; k < 4; ++k)
            mul_xd_minus_1(D, &degD, neg_divs[k]);

        /* Compute Φ₁₀₅(x) = N(x) / D(x) */
        seq_t Quot[49] = {0};
        size_t degQ;
        divide_polynomial(N, degN, D, degD, Quot, &degQ);

        /* Store the coefficients */
        for (size_t i = 0; i <= degQ; ++i)
            coeff[i] = Quot[i];
        init = true;
    }
    return coeff[(size_t)n];
}
