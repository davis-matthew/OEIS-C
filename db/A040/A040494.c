/*
 * A040494  Continued fraction for sqrt(517).
 * https://oeis.org/A040494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   Continued fraction coefficients of sqrt(517)
 * Range:     u8 none, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Validated: partial, pass, 84 terms through n = 83 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

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
        period = mem_new(seq_t, cap);

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
                period = mem_resize(seq_t, period, cap);
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
