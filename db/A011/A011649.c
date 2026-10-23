/*
 * A011649  102nd cyclotomic polynomial.
 * https://oeis.org/A011649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   a(n) = coefficient of x^n in the 102nd cyclotomic polynomial
 *            Φ102(x)
 * Range:     i8 n <= 32, i16 n <= 32, i32 n <= 32, i64 n <= 32, i128 n <= 32,
 *            big any n
 * Helpers:   div_minus_one, mul_minus_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/div_minus_one.h"
#include "../utils/mul_minus_one.h"

/* Sequence a(n) */
seq_t a(idx_t n)
{
    static bool init = false;
    static seq_t poly[200];
    static size_t deg = 0;

    if (!init) {
        /* initialise to the constant polynomial 1 */
        for (size_t i = 0; i < 200; ++i) poly[i] = 0;
        poly[0] = 1;
        deg = 0;

        /* factors with exponent +1 in the product formula */
        const uint64_t muls[] = {2, 3, 17, 102};
        const size_t num_muls = sizeof(muls) / sizeof(muls[0]);

        /* factors with exponent -1 in the product formula */
        const uint64_t divs[] = {1, 6, 34, 51};
        const size_t num_divs = sizeof(divs) / sizeof(divs[0]);

        for (size_t i = 0; i < num_muls; ++i)
            mul_minus_one(poly, &deg, muls[i]);

        for (size_t i = 0; i < num_divs; ++i)
            div_minus_one(poly, &deg, divs[i]);

        init = true;
    }

    if ((size_t)n <= deg)
        return poly[(size_t)n];
    else
        return 0;
}
