/*
 * A040391  Continued fraction for sqrt(412).
 * https://oeis.org/A040391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   continued fraction terms for sqrt(412)
 * Range:     u8 none, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Cached data for the period of the continued fraction of sqrt(412). */
    static bool initialized = false;
    static seq_t a0 = 0;          /* a(0) = ⌊√412⌋  = 20 */
    static seq_t period[20];      /* period terms a(1)..a(20) */
    static idx_t period_len = 0;  /* length of the period */

    if (!initialized) {
        initialized = true;

        /* First term: floor(√412) */
        a0 = floor_sqrt((seq_t)412);

        uint64_t m = 0;                /* m0 */
        uint64_t d = 1;                /* d0 */
        uint64_t a_val = (uint64_t)a0; /* a0 as 64‑bit */

        idx_t pos = 0;
        while (1) {
            uint64_t m_next = d * a_val - m;
            uint64_t d_next = (412 - m_next * m_next) / d;
            uint64_t a_next = ((uint64_t)a0 + m_next) / d_next;

            period[pos++] = (seq_t)a_next;

            /* Termination when a_n = 2·a0.  For sqrt(412) this is 40. */
            if (a_next == (uint64_t)a0 * 2) {
                period_len = pos;
                break;
            }

            m      = m_next;
            d      = d_next;
            a_val  = a_next;
        }
    }

    if (n == 0)
        return a0;

    idx_t idx = (n - 1) % period_len;
    return period[idx];
}
