/*
 * A040955  Continued fraction for sqrt(987).
 * https://oeis.org/A040955
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Validated: partial, pass, 84 terms through n = 83 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction terms of sqrt(987) */
/* a(0) = floor(sqrt(987)); thereafter the period repeats with a(1)..a(4) = 2,2,2,62 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"

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
    static bool init = false;
    static seq_t a0;
    static seq_t period[256];
    static idx_t period_len;

    if (!init) {
        /* initial term */
        a0 = floor_sqrt((seq_t)987);

        /* compute the periodic part of a continued fraction */
        seq_t N = (seq_t)987;
        seq_t m = 0;
        seq_t d = 1;
        seq_t a_term = a0;
        idx_t cnt = 0;

        while (1) {
            /* next triple (m,d,a) */
            m = d * a_term - m;
            d = (N - m * m) / d;
            a_term = (a0 + m) / d;

            period[cnt++] = a_term;
            if (a_term == 2 * a0)           /* period ends */
                break;
        }
        period_len = cnt;
        init = true;
    }

    if (n == 0)
        return a0;

    idx_t idx = (n - 1) % period_len;
    return period[idx];
}
