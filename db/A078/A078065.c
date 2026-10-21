/*
 * A078065  Expansion of (1-x)/(1+2*x+x^2+x^3).
 * https://oeis.org/A078065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 17, i32 n <= 37, i64 n <= 76, i128 n <= 155,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A078065 – expansion of (1‑x)/(1+2*x+x^2+x^3)
 * Recurrence:
 *   a(0) = 1
 *   a(1) = -3
 *   a(2) = 5
 *   a(n) = -2*a(n-1) - a(n-2) - a(n-3)   for n > 2
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-3;
    if (n == 2) return (seq_t)5;

    seq_t a0 = (seq_t)1;   /* a(n-3) */
    seq_t a1 = (seq_t)-3;  /* a(n-2) */
    seq_t a2 = (seq_t)5;   /* a(n-1) */
    seq_t ak = 0;

    for (idx_t i = 3; i <= n; ++i) {
        ak = (- (seq_t)2) * a2 - a1 - a0;
        /* shift for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = ak;
    }
    return ak;
}
