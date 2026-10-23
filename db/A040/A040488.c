/*
 * A040488  Continued fraction for sqrt(511).
 * https://oeis.org/A040488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A040488: continued fraction coefficients for sqrt(511).
   a(0) = floor(sqrt(511)) = 22, and for n ≥ 1 the terms repeat
   periodically with the sequence computed by the standard algorithm
   for quadratic surds. */
#include <stddef.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Precompute the periodic part of the continued fraction for sqrt(511).
       The period ends when the coefficient equals 2*a0. */
    static seq_t period[200];
    static size_t period_len = 0;

    if (period_len == 0) {
        const seq_t N = (seq_t)511;
        const seq_t a0 = floor_sqrt(N);
        seq_t m = 0;
        seq_t d = 1;
        seq_t a = a0;

        do {
            /* Next terms */
            m = d * a - m;
            d = (N - m * m) / d;
            a = (a0 + m) / d;
            period[period_len++] = a;
        } while (a != (seq_t)2 * a0);
    }

    if (n == 0) {
        return floor_sqrt((seq_t)511);
    }
    size_t idx = (size_t)((n - 1) % period_len);
    return period[idx];
}
