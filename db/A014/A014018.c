/*
 * A014018  Inverse of 9th cyclotomic polynomial.
 * https://oeis.org/A014018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-3) - a(n-6) * a(0) = 1, a(1) = 0, a(2) = 0 (period
 *            9)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t period[9];
    static bool initialized = false;

    if (!initialized) {
        /* Compute the first 9 values by the recurrence */
        for (idx_t i = 0; i < 9; ++i) {
            if (i == 0) {
                period[i] = (seq_t)1;
            } else {
                seq_t prev3 = (i >= 3) ? period[i-3] : (seq_t)0;
                seq_t prev6 = (i >= 6) ? period[i-6] : (seq_t)0;
                period[i] = -prev3 - prev6;
            }
        }
        initialized = true;
    }

    return period[n % 9];
}
