/*
 * A040627  Continued fraction for sqrt(653).
 * https://oeis.org/A040627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction coefficients of √653
   a(0) = ⌊√653⌋,
   a(n) for n≥1 are the periodic part of the continued fraction of √653. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/floor_sqrt.h"   /* provides seq_t floor_sqrt(seq_t) */

seq_t a(idx_t n)
{
    /* lazy initialization of the period of the continued fraction */
    static bool init = false;
    static seq_t period[256];          /* sufficient for the expected period length */
    static idx_t period_len = 0;
    static seq_t a0_cached = 0;

    if (!init) {
        const seq_t D = 653;           /* the non‑square integer */
        seq_t a0 = floor_sqrt(D);     /* a0 = ⌊√D⌋ */
        a0_cached = a0;

        seq_t m    = 0;
        seq_t d    = 1;
        seq_t a_prev = a0;

        /* Generate the periodic part until 2*a0 is reached */
        for (;;) {
            /* m_{k+1} = d_k * a_k - m_k */
            seq_t m_next = d * a_prev - m;

            /* d_{k+1} = (D - m_{k+1}^2) / d_k */
            seq_t d_next = (D - m_next * m_next) / d;

            /* a_{k+1} = ⌊(a0 + m_{k+1}) / d_{k+1}⌋ */
            seq_t a_next = (a0 + m_next) / d_next;

            period[period_len++] = a_next;
            if (a_next == a0 * 2)          /* end of period for sqrt of a non‑square */
                break;

            m      = m_next;
            d      = d_next;
            a_prev = a_next;
            /* safety guard: period_len should never exceed 256 for D=653 */
        }

        init = true;
    }

    if (n == 0)
        return a0_cached;

    /* index into the periodic part (period starts at a(1)) */
    idx_t idx = (n - 1) % period_len;
    return period[idx];
}
