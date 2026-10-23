/*
 * A040494  Continued fraction for sqrt(517).
 * https://oeis.org/A040494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   Continued fraction coefficients of sqrt(517)
 * Range:     u8 none, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t a0 = 0;
    static seq_t *period = NULL;
    static size_t period_len = 0;

    if (!initialized)
    {
        /* Compute a0 = floor(sqrt(517)) */
        seq_t D = 517;
        a0 = floor_sqrt(D);

        /* Generate the period of the continued fraction */
        seq_t m = 0;
        seq_t d = 1;
        seq_t a_term = a0;          /* current a value */
        size_t cap = 64;
        period = (seq_t *)malloc(cap * sizeof(seq_t));

        while (1)
        {
            /* next values in the algorithm */
            seq_t m_next = d * a_term - m;
            seq_t d_next = (D - m_next * m_next) / d;
            seq_t a_next = (a0 + m_next) / d_next;

            /* store a_next as part of the period */
            if (period_len == cap)
            {
                cap *= 2;
                period = (seq_t *)realloc(period, cap * sizeof(seq_t));
            }
            period[period_len++] = a_next;

            /* terminate when the period repeats */
            if (a_next == 2 * a0)
                break;

            /* advance to next step */
            m = m_next;
            d = d_next;
            a_term = a_next;
        }
        initialized = true;
    }

    if (n == 0)
        return a0;

    idx_t pos = (n - 1) % period_len;
    return period[pos];
}
