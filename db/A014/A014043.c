/*
 * A014043  Inverse of 34th cyclotomic polynomial.
 * https://oeis.org/A014043
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 1, a(j) = 0 for j < 0, a(n) = sum_{k=1}^{16} (-1)^{k+1} *
 *            a(n-k) for n ≥ 1. The sequence is periodic with period 34
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache of the first 34 terms of the period */
    static seq_t period[34];
    static bool initialized = false;

    if (!initialized) {
        /* a(0) = 1 */
        period[0] = (seq_t)1;

        /* compute terms 1..33 via the recurrence */
        for (idx_t i = 1; i < 34; ++i) {
            seq_t sum = (seq_t)0;
            int sign = 1;                      /* starts +1 for a(n-1) */
            for (idx_t offset = 1; offset <= 16; ++offset) {
                if (i < offset)                /* a(j)=0 for j<0 */
                    continue;

                seq_t val = period[i - offset];
                if (sign > 0)
                    sum += val;
                else
                    sum -= val;

                sign = -sign;                  /* alternate sign */
            }
            period[i] = sum;
        }
        initialized = true;
    }

    return period[n % 34];
}
