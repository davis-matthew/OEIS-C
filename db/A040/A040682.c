/*
 * A040682  Continued fraction for sqrt(709).
 * https://oeis.org/A040682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = the n‑th term of the continued fraction expansion of
 *            √709, with a(0) = ⌊√709⌋ = 26
 * Range:     u8 none, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Validated: partial, pass, 90 terms through n = 89 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
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
    static seq_t period[64];
    static size_t period_len = 0;
    const seq_t S = (seq_t)709;

    if (!init) {
        /* Compute the periodic part of the continued fraction */
        seq_t a0 = floor_sqrt(S);   /* ⌊√709⌋ = 26 */
        seq_t m = 0;
        seq_t d = 1;
        seq_t a = a0;
        size_t idx = 0;

        while (1) {
            /* Next continued‑fraction term */
            seq_t m_next = d * a - m;
            seq_t d_next = (S - m_next * m_next) / d;
            seq_t a_next = (a0 + m_next) / d_next;

            period[idx++] = a_next;

            /* 2·a0 appears exactly at the end of the period */
            if (a_next == a0 * 2) {
                period_len = idx;
                break;
            }

            m = m_next;
            d = d_next;
            a = a_next;
        }
        init = true;
    }

    if (n == 0)
        return floor_sqrt(S);

    size_t pos = ((size_t)(n - 1)) % period_len;
    return period[pos];
}
