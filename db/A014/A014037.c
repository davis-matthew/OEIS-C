/*
 * A014037  Inverse of 28th cyclotomic polynomial.
 * https://oeis.org/A014037
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-2) - a(n-4) + a(n-6) - a(n-8) + a(n-10) - a(n-12),
 *            with a(0) = 1 and a(k) = 0 for k < 0. The sequence is periodic
 *            with period 28
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Precompute the period once */
    static int initialized = 0;
    static seq_t period[28];

    if (!initialized) {
        initialized = 1;
        period[0] = 1;
        for (idx_t i = 1; i < 28; ++i) {
            seq_t val = 0;
            if (i >= 2)  val += period[i - 2];
            if (i >= 4)  val -= period[i - 4];
            if (i >= 6)  val += period[i - 6];
            if (i >= 8)  val -= period[i - 8];
            if (i >= 10) val += period[i - 10];
            if (i >= 12) val -= period[i - 12];
            period[i] = val;
        }
    }

    /* Return the value for the given n modulo the period length */
    size_t idx = (size_t)(n % 28ULL);
    return period[idx];
}
