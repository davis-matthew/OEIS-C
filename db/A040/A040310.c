/*
 * A040310  Continued fraction for sqrt(329).
 * https://oeis.org/A040310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Periodic continued fraction terms for sqrt(329).  Period length = 12. */

#include <stddef.h>  /* for size_t */

seq_t a(idx_t n)
{
    /* Static state for lazy initialization. */
    static seq_t a0 = 0;          /* first term */
    static idx_t period_len = 0;  /* length of the period (12) */
    static seq_t period[64];      /* buffer for the period, 64 is safe */

    /* One-time initialization. */
    if (period_len == 0) {
        /* Base term sqrt floor(329) = 18. */
        a0 = 18;

        /* Compute the periodic part using the standard algorithm for
           continued fractions of quadratic surds. */
        const seq_t D = 329;       /* the radicand */
        seq_t m = 0;
        seq_t d = 1;
        seq_t a = a0;

        /* The period ends when the term equals 2*a0 (36). */
        do {
            /* Next values of m, d, a. */
            seq_t m_tmp = d * a - m;
            m = m_tmp;

            seq_t d_tmp = (D - m * m) / d;
            d = d_tmp;

            a = (a0 + m) / d;

            period[period_len++] = a;
        } while (a != (seq_t)(2 * a0));
    }

    /* Return the requested term. */
    if (n == 0)
        return a0;

    /* n ≥ 1: pick the element in the periodic block. */
    idx_t idx = (n - 1) % period_len;
    return period[(size_t)idx];
}
