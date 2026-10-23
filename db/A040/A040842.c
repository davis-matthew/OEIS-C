/*
 * A040842  Continued fraction for sqrt(872).
 * https://oeis.org/A040842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = floor(sqrt(872)) = 29. For n ≥ 1, a(n) are the periodic
 *            terms of the continued‑fraction expansion of sqrt(872). The
 *            period starts with a(1)
 * Range:     u8 none, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* static state – computed on first call only */
    static idx_t a0 = 0;          /* floor(sqrt(872))          */
    static seq_t *period = NULL;  /* one full period of the CF */
    static idx_t period_len = 0;  /* length of that period    */

    if (period_len == 0) {
        /* compute a0 */
        seq_t tmp = floor_sqrt((seq_t)872);
        a0 = (idx_t)tmp;

        /* compute the full period (standard algorithm) */
        const idx_t D = 872;
        idx_t m = 0;
        idx_t d = 1;
        idx_t prev_a = a0;   /* a_{k-1};  k=1 uses a0          */

        size_t cap = 16;
        period = (seq_t *)malloc(cap * sizeof(seq_t));
        idx_t len = 0;

        while (true) {
            idx_t m_next = d * prev_a - m;
            idx_t d_next = (D - m_next * m_next) / d;
            idx_t a_next = (a0 + m_next) / d_next;

            if (len == (idx_t)cap) {
                cap *= 2;
                period = (seq_t *)realloc(period, cap * sizeof(seq_t));
            }
            period[len++] = (seq_t)a_next;

            m = m_next;
            d = d_next;
            prev_a = a_next;

            if (m == a0 && d == 1)  /* period completed */
                break;
        }
        period_len = len;
    }

    if (n == 0)
        return (seq_t)a0;

    idx_t pos = (n - 1) % period_len;
    return period[pos];
}
