/*
 * A188802  Expansion of (x^2+1)/(x^4+2*x^3-2*x+1).
 * https://oeis.org/A188802
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 8, i16 n <= 20, i32 n <= 41, i64 n <= 83, i128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A188802: Radix expansion of (x^2+1)/(x^4+2*x^3-2*x+1)
 *
 * Recurrence (derived from the numerator/denominator polynomial):
 *   a(0) = 1
 *   a(1) = 2
 *   a(2) = 5
 *   a(3) = 8
 *   for n ≥ 4:
 *     a(n) = 2*a(n-1) - 2*a(n-3) - a(n-4)
 *
 * The recurrence uses only integer arithmetic; no floating‑point or
 * external helper functions are required.
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)8;

    /* Use a circular buffer for the last four values. */
    seq_t buf[4];
    buf[0] = (seq_t)1;   /* a(0) */
    buf[1] = (seq_t)2;   /* a(1) */
    buf[2] = (seq_t)5;   /* a(2) */
    buf[3] = (seq_t)8;   /* a(3) */

    for (idx_t i = 4; i <= n; ++i) {
        /* Indices of needed previous terms in the circular buffer. */
        idx_t im1 = (i - 1) % 4;   /* a(i-1) */
        idx_t im3 = (i - 3) % 4;   /* a(i-3) */
        idx_t im4 = (i - 4) % 4;   /* a(i-4) */

        /* Compute a(i) via the recurrence. */
        seq_t next = 2*buf[im1] - 2*buf[im3] - buf[im4];

        /* Store the new value into the buffer, overwriting the oldest. */
        buf[i % 4] = next;
    }

    /* Return the value corresponding to a(n). */
    return buf[n % 4];
}
