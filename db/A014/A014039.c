/*
 * A014039  Inverse of 30th cyclotomic polynomial.
 * https://oeis.org/A014039
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficients of the power‑series expansion of 1/(1 + x -
 *            x^3 - x^4 - x^5 + x^7 + x^8). The coefficients satisfy the
 *            linear recurrence a(n) = -a(n-1) + a(n-3) + a(n-4) + a(n-5) -
 *            a(n-7) - a(n-8), with a(0)=1 and a(k)=0 for k<0. The sequence is
 *            periodic with period 30
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Period stored after the first call. */
    static seq_t period[30];
    static int initialized = 0;

    if (!initialized) {
        /* initial values for negative indices are implicitly zero */
        period[0] = 1;
        for (idx_t k = 1; k < 30; ++k) {
            seq_t val = 0;
            /* a(k-1) term (always exists for k>=1) */
            val -= period[(size_t)(k-1)];
            if (k >= 3) val += period[(size_t)(k-3)];
            if (k >= 4) val += period[(size_t)(k-4)];
            if (k >= 5) val += period[(size_t)(k-5)];
            if (k >= 7) val -= period[(size_t)(k-7)];
            if (k >= 8) val -= period[(size_t)(k-8)];
            period[(size_t)k] = val;
        }
        initialized = 1;
    }

    size_t idx = (size_t)(n % 30);
    return period[idx];
}
