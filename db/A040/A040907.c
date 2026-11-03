/*
 * A040907  Continued fraction for sqrt(938).
 * https://oeis.org/A040907
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   continued fraction terms for sqrt(938) a(0) = floor(sqrt(938)) =
 *            30 For n >= 1, a(n) follows the repeating period [1, 1, 1, 2, 8,
 *            2, 1, 1, 1, 60]
 * Range:     u8 none, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Validated: partial, pass, 90 terms through n = 89 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
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
    static bool initialized = false;
    static seq_t a0 = 0;
    static seq_t period[20];
    static size_t period_len = 0;

    if (!initialized) {
        a0 = floor_sqrt((seq_t)938);
        const seq_t N = (seq_t)938;

        seq_t m = 0;
        seq_t d = 1;
        seq_t ak = a0;

        for (;;) {
            m = d * ak - m;
            d = (N - m * m) / d;
            ak = (a0 + m) / d;

            period[period_len++] = ak;

            if (ak == 2 * a0)
                break;
        }

        initialized = true;
    }

    if (n == 0)
        return a0;

    size_t idx = (size_t)((n - 1) % period_len);
    return period[idx];
}
