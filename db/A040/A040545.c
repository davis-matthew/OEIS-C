/*
 * A040545  Continued fraction for sqrt(569).
 * https://oeis.org/A040545
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   continued fraction terms of sqrt(569)
 * Range:     u8 none, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* The first term a(0) is the floor of sqrt(569). */
    const seq_t a0 = 23;
    const idx_t   S = 569;   /* the radicand */

    /* Compute and cache one period of the continued fraction. */
    static seq_t *period   = NULL;
    static size_t period_len = 0;

    if (!period) {
        /* The period length for sqrt(N) is < 2*sqrt(N), so this
           allocation is safe for the fixed radicand 569. */
        size_t cap = 2 * (size_t)a0 + 1;  /* 47 */
        period = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!period) abort();

        seq_t m    = 0;
        seq_t d    = 1;
        seq_t a_cur = a0;

        size_t i = 0;
        while (true) {
            seq_t m_next = d * a_cur - m;
            seq_t d_next = (seq_t)(S - m_next * m_next) / d;
            seq_t a_next = (a0 + m_next) / d_next;

            period[i++] = a_next;

            if (a_next == 2 * a0) {   /* period terminates */
                period_len = i;
                break;
            }

            /* Prepare for next iteration. */
            m    = m_next;
            d    = d_next;
            a_cur = a_next;
        }
    }

    if (n == 0)
        return a0;

    /* cycle through the period (indices start at 1). */
    return period[(size_t)(n - 1) % period_len];
}
