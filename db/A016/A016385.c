/*
 * A016385  330th cyclotomic polynomial.
 * https://oeis.org/A016385
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of xⁿ in Φ₃₃₀(x) Φ₃₃₀(x) = ∏_{d∣330} (xᵈ –
 *            1)^{μ(330/d)}
 * Range:     i8 n <= 80, i16 n <= 80, i32 n <= 80, i64 n <= 80, i128 n <= 80,
 *            big any n
 * Helpers:   mul_by_factor_int64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mul_by_factor_int64.h"

seq_t a(idx_t n)
{
    /* Divisors of 330 and the corresponding μ(330/d) values */
    static const idx_t DIV[16] = {1, 2, 3, 5, 6, 10, 11, 15, 22, 30,
                                  33, 55, 66, 110, 165, 330};
    static const int   MU[16]  = { 1, -1, -1, -1,  1,  1, -1,  1,  1, -1,
                                   1,  1, -1, -1, -1,  1};

    static bool ready = false;
    static seq_t coeff[81];

    if (!ready) {
        /* Build numerator and denominator polynomials using 64‑bit ints */
        int64_t num[500] = {0};
        int64_t den[500] = {0};
        size_t degN = 0, degD = 0;
        num[0] = 1;
        den[0] = 1;

        for (size_t i = 0; i < 16; ++i) {
            idx_t d   = DIV[i];
            int   exp = MU[i];
            if (exp ==  1)
                mul_by_factor_int64(num, &degN, (size_t)d);
            else if (exp == -1)
                mul_by_factor_int64(den, &degD, (size_t)d);
        }

        /* polynomial division: num = den * quotient, deg difference is 80 */
        size_t degQ = degN - degD;          /* expected 80 */
        int64_t q[81] = {0};

        for (size_t i = 0; i <= degQ; ++i) {
            int64_t val = num[i];
            size_t maxj = (i < degD) ? i : degD;
            for (size_t j = 1; j <= maxj; ++j)
                val -= den[j] * q[i - j];
            q[i] = val;
        }

        for (size_t i = 0; i <= degQ; ++i)
            coeff[i] = (seq_t)q[i];
        ready = true;
    }

    if (n < 81)
        return coeff[(size_t)n];
    return (seq_t)0;
}
