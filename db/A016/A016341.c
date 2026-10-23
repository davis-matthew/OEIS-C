/*
 * A016341  161st cyclotomic polynomial.
 * https://oeis.org/A016341
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of x^n in the 161st cyclotomic polynomial *
 *            Φ₁₆₁(x) = (x¹⁶² - x¹⁶¹ - x + 1) / (x³⁰ - x²³ - x⁷ + 1)
 * Range:     i8 n <= 132, i16 n <= 132, i32 n <= 132, i64 n <= 132,
 *            i128 n <= 132, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* coefficients of Φ₁₆₁(x) stored once */
    static seq_t coeff[133] = {0};
    static bool initialized = false;

    if (!initialized) {
        /* degrees */
        const idx_t degN = 162;   /* numerator degree */
        const idx_t degD = 30;    /* denominator degree */
        const idx_t degQ = degN - degD; /* resulting degree = 132 */

        /* numerator N(x) = x^162 - x^161 - x + 1 */
        seq_t rem[163] = {0};
        rem[0]  = 1;    /* +1 at x^0 */
        rem[1]  = -1;   /* -1 at x^1 */
        rem[161] = -1;  /* -1 at x^161 */
        rem[162] = 1;   /* +1 at x^162 */

        /* denominator D(x) = x^30 - x^23 - x^7 + 1 */
        seq_t den[31] = {0};
        den[0]  = 1;    /* +1 at x^0 */
        den[7]  = -1;   /* -1 at x^7 */
        den[23] = -1;   /* -1 at x^23 */
        den[30] = 1;    /* +1 at x^30 */

        /* quotient Q(x) = N(x) / D(x) */
        seq_t qs[133] = {0};

        /* determine highest non‑zero degree of remainder */
        idx_t remDeg = degN;
        while (remDeg > 0 && rem[remDeg] == 0) remDeg--;

        /* long division */
        while (remDeg >= degD) {
            idx_t diff  = remDeg - degD;
            seq_t coeff = rem[remDeg];   /* leading coefficient of rem is ±1 */
            qs[diff] = coeff;

            /* rem -= coeff * x^diff * den */
            for (idx_t j = 0; j <= degD; ++j) {
                rem[j + diff] -= coeff * den[j];
            }

            /* update remDeg (skip possible vanished high terms) */
            while (remDeg > 0 && rem[remDeg] == 0) remDeg--;
        }

        /* copy result into static coefficient array */
        for (idx_t i = 0; i <= degQ; ++i) coeff[i] = qs[i];
        initialized = true;
    }

    /* out‑of‑range indices produce 0 */
    if (n >= 133) return 0;
    return coeff[n];
}
