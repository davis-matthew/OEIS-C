/*
 * A040578  Continued fraction for sqrt(603).
 * https://oeis.org/A040578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(n) is the n‑th term of the continued fraction expansion of
 *            sqrt(603)
 * Range:     u8 none, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Validated: partial, pass, 85 terms through n = 84 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
    static seq_t period[200];
    static idx_t period_len = 0;
    static seq_t a0;

    /* Compute the period of the continued fraction the first time */
    if (period_len == 0) {
        a0 = floor_sqrt((seq_t)603);
        seq_t m = 0;
        seq_t d = 1;
        seq_t a_cur = a0;
        idx_t i = 0;
        seq_t D = (seq_t)603;
        seq_t twice_a0 = a0 * 2;

        while (1) {
            m = d * a_cur - m;
            d = (D - m * m) / d;
            a_cur = (a0 + m) / d;
            period[i++] = a_cur;
            if (a_cur == twice_a0)
                break;
        }
        period_len = i;
    }

    if (n == 0)
        return a0;
    idx_t idx = (n - 1) % period_len;
    return period[idx];
}
