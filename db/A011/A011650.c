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
 * Helpers:   divide_polynomial, mul_xd_minus_1
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/divide_polynomial.h"
#include "../utils/mul_xd_minus_1.h"

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
