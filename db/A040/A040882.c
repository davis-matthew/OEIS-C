/*
 * A040882  Continued fraction for sqrt(913).
 * https://oeis.org/A040882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction terms for sqrt(913).
   a(0) = floor(sqrt(913)),
   subsequent terms are produced by the standard recurrence for quadratic surds. */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    static bool init = false;
    static seq_t period[128];
    static size_t period_len = 0;
    static seq_t a0 = 0;

    if (!init) {                     /* compute the repeating part once */
        const seq_t N = (seq_t)913u;  /* discriminant D = 913 */
        a0 = floor_sqrt(N);          /* integer part of sqrt(913) */
        seq_t m = (seq_t)0;
        seq_t d = (seq_t)1;
        seq_t a_term = a0;
        size_t i = 0;

        while (1) {
            m = d * a_term - m;
            d = (N - m * m) / d;
            a_term = (a0 + m) / d;   /* floor division */
            period[i++] = a_term;
            if (d == (seq_t)1)       /* period ends when d returns to 1 */
                break;
            if (i >= 128)            /* safety guard, should not happen */
                break;
        }
        period_len = i;
        init = true;
    }

    if (n == 0)                 /* offset 0: first term is a0 */
        return a0;

    size_t idx = (size_t)((n - 1) % period_len); /* repeat period */
    return period[idx];
}
