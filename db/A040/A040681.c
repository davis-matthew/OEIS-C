/*
 * A040681  Continued fraction for sqrt(708).
 * https://oeis.org/A040681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = coefficient of the continued fraction expansion of
 *            sqrt(708). The sequence starts at a(0)=26 and is periodic with
 *            period length 12
 * Range:     u8 none, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Validated: partial, pass, 90 terms through n = 89 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
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
    static seq_t *period = NULL;
    static size_t period_len = 0;
    static seq_t a0 = 0;
    static bool initialized = false;

    if (!initialized) {
        /* Compute the continued‑fraction coefficients of sqrt(708). */
        const seq_t D = (seq_t)708;
        a0 = floor_sqrt(D);

        seq_t m = 0;
        seq_t d = 1;
        seq_t a_k = a0;

        size_t cap = 4;
        period = mem_new(seq_t, cap);
        period_len = 0;

        while (true) {
            /* next triple (m, d, a) */
            m = d * a_k - m;
            d = (D - m * m) / d;
            a_k = (a0 + m) / d;

            /* store the coefficient a_k (excluding the initial a0) */
            if (period_len == cap) {
                cap <<= 1;
                period = mem_resize(seq_t, period, cap);
            }
            period[period_len++] = a_k;

            if (a_k == 2 * a0)  /* end of period */
                break;
        }
        initialized = true;
    }

    if (n == 0)
        return a0;

    /* periodic part: indices 0..period_len-1 correspond to a(1)..a(period_len) */
    idx_t idx = (n - 1) % period_len;
    return period[idx];
}
