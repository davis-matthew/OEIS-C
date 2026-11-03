/*
 * A040898  Continued fraction for sqrt(929).
 * https://oeis.org/A040898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 86 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of the continued‑fraction coefficients for sqrt(929).
   a(0) = ⌊√929⌋, and for n > 0, a(n) are the periodic terms of the
   continued fraction of √929. */
#include <stddef.h>
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
    /* one‑time static data */
    static bool initialized = false;
    static seq_t period[128];
    static size_t period_len = 0;
    static seq_t a0 = 0;

    if (!initialized) {
        /* compute the integer part of √929 */
        const seq_t N = (seq_t)929;
        a0 = floor_sqrt(N);

        seq_t m = 0;
        seq_t d = 1;
        seq_t a_cur = a0;

        /* generate the periodic part until the term equals 2*a0 */
        while (1) {
            m = d * a_cur - m;
            seq_t num = N - m * m;
            d = num / d;
            a_cur = (a0 + m) / d;

            period[period_len++] = a_cur;
            if (a_cur == 2 * a0) break;
        }

        initialized = true;
    }

    if (n == 0) return a0;

    /* wrap around the period */
    idx_t idx = (n - 1) % period_len;
    return period[(size_t)idx];
}
