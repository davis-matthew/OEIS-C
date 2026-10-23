/*
 * A040492  Continued fraction for sqrt(515).
 * https://oeis.org/A040492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction coefficients for sqrt(515).  The sequence is
   periodic after the first term: a(0) = 22, and then the period of
   length 10 begins: 1, 2, 3, 1, 3, 1, 3, 2, 1, 44. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Fundamental constants */
    static const seq_t a0 = (seq_t)22;          /* floor(sqrt(515)) */
    static const seq_t D  = (seq_t)515;         /* The square‑free part */

    /* Periodic part of the continued fraction */
    static seq_t period[128];
    static idx_t period_len = 0;
    static bool initialized = false;

    /* Build the period once, on first call */
    if (!initialized) {
        seq_t m = 0;
        seq_t d = 1;
        seq_t a = a0;
        idx_t idx = 0;

        /* Generate a1 .. aperiod until a = 2*a0, which signals the end
           of the period for square‑root continued fractions. */
        do {
            m = d * a - m;
            d = (D - m * m) / d;
            a = (a0 + m) / d;
            period[idx++] = a;
        } while (a != 2 * a0);

        period_len = idx;
        initialized = true;
    }

    /* Return the requested term */
    if (n == 0)
        return a0;

    idx_t pos = (n - 1) % period_len;
    return period[pos];
}
