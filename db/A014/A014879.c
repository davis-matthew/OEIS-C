/*
 * A014879  Inverse of 870th cyclotomic polynomial.
 * https://oeis.org/A014879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in 1/Φ_870(x); the sequence is
 *            periodic of period 870
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define WIDTH 870

/* Divisors of 870 */
static const int finite_divs[8]   = {  2,  3,  5, 29, 30, 174, 290, 435 };
static const int infinite_divs[8] = {  1,  6, 10, 15, 58, 87, 145, 870 };

seq_t a(idx_t n)
{
    static seq_t coeff[WIDTH];
    static bool initialized = false;

    if (!initialized) {
        int i, j, pos;
        seq_t tmp[WIDTH];

        /* Initialise the coefficient array */
        for (i = 0; i < WIDTH; ++i) coeff[i] = 0;
        coeff[0] = 1;

        /* Multiply by all finite factors (1 - x^d) */
        for (j = 0; j < 8; ++j) {
            int d = finite_divs[j];
            for (i = WIDTH - 1; i >= d; --i)
                coeff[i] -= coeff[i - d];
        }

        /* Multiply by all infinite factors 1/(1 - x^d) */
        for (j = 0; j < 8; ++j) {
            int d = infinite_divs[j];

            /* initialise temporary array to zero */
            for (i = 0; i < WIDTH; ++i) tmp[i] = 0;

            /* convolution with 1 + x^d + x^(2d) + ... */
            for (i = 0; i < WIDTH; ++i) {
                seq_t val = coeff[i];
                if (val == 0) continue;
                for (pos = i; pos < WIDTH; pos += d)
                    tmp[pos] += val;
            }

            /* copy back the result into coeff */
            for (i = 0; i < WIDTH; ++i) coeff[i] = tmp[i];
        }

        initialized = true;
    }

    return coeff[(idx_t)(n % WIDTH)];
}
