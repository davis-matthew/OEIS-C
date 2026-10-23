/*
 * A040298  Continued fraction for sqrt(316).
 * https://oeis.org/A040298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Continued fraction coefficients for sqrt(316) */

#include <stddef.h>

static seq_t period_arr[8];
static size_t period_len = 0;

/* Compute the periodic part of the continued fraction of sqrt(316) */
static void init_period(void) {
    const seq_t a0 = 17;
    seq_t m = 0;
    seq_t d = 1;
    seq_t a_cur = a0;
    size_t idx = 0;

    while (1) {
        seq_t m_next = d * a_cur - m;
        seq_t d_next = ((seq_t)316 - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;

        period_arr[idx++] = a_next;          /* store coefficient */
        if (a_next == 2 * a0) {              /* 2*a0 = 34, end of period */
            period_len = idx;
            break;
        }

        m = m_next;
        d = d_next;
        a_cur = a_next;
    }
}

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)17;           /* a(0) = 17 */

    if (period_len == 0)
        init_period();

    /* Index into the periodic part: a(1) → period_arr[0] */
    size_t pos = (size_t)((n - 1) % (idx_t)period_len);
    return period_arr[pos];
}
