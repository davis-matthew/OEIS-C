/*
 * A040217  Continued fraction for sqrt(233).
 * https://oeis.org/A040217
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n-th partial quotient in the continued‑fraction expansion of sqrt(233) (a(0)=15) */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Integer part of sqrt(233) */
    const seq_t a0_seq = 15;
    const uint64_t a0      = 15;
    static bool initialized = false;
    static seq_t *period   = NULL;
    static size_t period_len = 0;

    if (!initialized) {
        uint64_t m = 0;
        uint64_t d = 1;
        uint64_t a_term = a0;
        /* Temporary buffer for the period */
        seq_t tmp[256];
        size_t idx = 0;
        while (1) {
            m = d * a_term - m;
            d = (233ULL - m * m) / d;
            a_term = (a0 + m) / d;
            tmp[idx++] = (seq_t)a_term;
            if (a_term == 2 * a0)      /* end of period for sqrt(D) */
                break;
        }
        period_len = idx;
        period = (seq_t *)malloc(period_len * sizeof(seq_t));
        if (!period) abort();        /* unlikely, but safe */
        for (size_t i = 0; i < period_len; ++i)
            period[i] = tmp[i];
        initialized = true;
    }

    if (n == 0)
        return a0_seq;

    size_t pos = (size_t)((n - 1) % period_len);
    return period[pos];
}
