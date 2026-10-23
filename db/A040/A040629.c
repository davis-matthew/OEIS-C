/*
 * A040629  Continued fraction for sqrt(655).
 * https://oeis.org/A040629
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(0) = ⌊√655⌋, a(n) for n > 0 are the terms of the
 *            continued‑fraction expansion of √655 (periodic, last term
 *            2·a(0))
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

static seq_t isqrt_idx(idx_t x) {
    idx_t lo = 0, hi = x, ans = 0;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        __int128 sq = (__int128)mid * mid;
        if (sq <= x) {
            ans = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return (seq_t)ans;
}

seq_t a(idx_t n) {
    static bool initialized = false;
    static seq_t a0 = 0;
    static seq_t period[64];
    static size_t period_len = 0;

    if (!initialized) {
        const idx_t D = 655;
        a0 = isqrt_idx(D);
        idx_t a0_i = (idx_t)a0;

        idx_t m = 0, d = 1, a_val = a0_i;
        period_len = 0;
        do {
            m = d * a_val - m;
            d = (D - (__int128)m * m) / d;
            a_val = (a0_i + m) / d;
            period[period_len++] = (seq_t)a_val;
        } while (a_val != 2 * a0_i);
        initialized = true;
    }

    if (n == 0) return a0;
    idx_t idx = (n - 1) % period_len;
    return period[(size_t)idx];
}
