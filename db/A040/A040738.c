/*
 * A040738  Continued fraction for sqrt(766).
 * https://oeis.org/A040738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A040738: Continued fraction coefficients of sqrt(766) */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Radicand and integer part of sqrt(766) */
    const idx_t D       = 766u;
    const idx_t a0_val  = 27u;           /* integer part, fits in idx_t */
    const seq_t a0_seq  = 27;            /* same value for seq_t */

    /* Periodic part of the continued fraction */
    static seq_t period[200];
    static size_t period_len = 0;
    static bool   initialized = false;

    if (!initialized) {
        idx_t m      = 0;
        idx_t d      = 1;
        idx_t a_val  = a0_val;
        size_t i     = 0;

        /* Unfold (m,d,a) for continued fractions of a quadratic irrational
           until the period ends when a_val becomes 2*a0_val. */
        do {
            m     = d * a_val - m;
            d     = (D - m * m) / d;
            a_val = (a0_val + m) / d;
            period[i++] = (seq_t) a_val;
        } while (a_val != 2 * a0_val);

        period_len = i;
        initialized = true;
    }

    if (n == 0)
        return a0_seq;

    idx_t idx = (n - 1) % period_len;      /* n>=1 gives a safe idx */
    return period[(size_t) idx];
}
