/*
 * A015032  Inverse of 1023rd cyclotomic polynomial.
 * https://oeis.org/A015032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in ∏_{d|1023} (1 - x^d)^{-µ(1023/d)}
 *            (period 1023)
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>


seq_t a(idx_t n) {
    const idx_t period = 1023;
    static seq_t coeff[1023];
    static int ready = 0;

    if (!ready) {
        const int L = 1023 - 1;                /* highest needed index */
        seq_t cur[1023];
        seq_t temp[1023];

        /* initialise polynomial to 1 */
        cur[0] = 1;
        for (size_t i = 1; i < 1023; ++i) cur[i] = 0;

        /* divisors of 1023 and their μ(1023/d) values */
        static const idx_t divs[8] = {1, 3, 11, 33, 31, 93, 341, 1023};
        static const int mu[8]    = {-1, 1, 1, -1, 1, -1, -1, 1};

        for (int k = 0; k < 8; ++k) {
            idx_t d = divs[k];
            int m   = mu[k];

            if (m == 1) {
                /* multiply by 1/(1 - x^d) = 1 + x^d + x^{2d} + … */
                for (size_t i = 0; i < 1023; ++i) temp[i] = cur[i];
                for (int i = d; i <= L; ++i) {
                    temp[i] = temp[i] + temp[i - d];
                }
                for (size_t i = 0; i < 1023; ++i) cur[i] = temp[i];
            } else { /* m == -1, multiply by (1 - x^d) */
                for (int i = L; i >= (int)d; --i) {
                    cur[i] = cur[i] - cur[i - d];
                }
            }
        }

        for (size_t i = 0; i < 1023; ++i) coeff[i] = cur[i];
        ready = 1;
    }

    idx_t pos = n % period;   /* coefficient repeats every 1023 terms */
    return coeff[pos];
}
