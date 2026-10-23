/*
 * A040938  Continued fraction for sqrt(970).
 * https://oeis.org/A040938
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   compute_a0
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: continued‐fraction terms for sqrt(970) */
#include <stdint.h>
#include <stddef.h>
#include "../utils/compute_a0.h"

/* D = 970 */
static const seq_t D = (seq_t)970;

/* The expansion of sqrt(970) has period length 9 (including the
   terminating 2*a0).  We compute the period once and cache the
   values. */
static size_t period_len = 0;
static seq_t period[16];

/* Main function called by the test harness. */
seq_t a(idx_t n) {
    /* Compute a0 and period on first call. */
    if (period_len == 0) {
        seq_t a0 = compute_a0();

        seq_t m = 0;
        seq_t d = 1;
        seq_t ak = a0;  /* the first term after a0 */

        do {
            /* Next m */
            seq_t m_next = d * ak - m;

            /* d_next = (D - m_next^2) / d  */
            uint32_t delta = (uint32_t)D - (uint32_t)m_next * (uint32_t)m_next;
            seq_t d_next = (seq_t)(delta / d);

            /* a_next = floor((a0 + m_next)/d_next) */
            seq_t a_next = (a0 + m_next) / d_next;

            period[period_len++] = a_next;

            m = m_next;
            d = d_next;
            ak = a_next;
        } while (ak != (seq_t)(2 * a0));  /* 2*a0 terminates the period */
    }

    if (n == 0) {
        /* Return a0, the leading term */
        return compute_a0();
    }

    /* Return the (n-1)th term of the periodic part. */
    idx_t index = (n - 1) % (idx_t)period_len;
    return period[(size_t)index];
}
