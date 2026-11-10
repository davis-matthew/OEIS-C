/*
 * A071018  Inverse Moebius transform of A002487.
 * https://oeis.org/A071018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|(n+1)} A002487(d+1), where A002487 is Stern's
 *            diatomic series: A002487(0) = 0, A002487(1) = 1, A002487(2k) =
 *            A002487(k), A002487(2k+1) = A002487(k) + A002487(k+1)
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Validated: partial, pass, 82 terms through n = 81 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A071018_stern(idx_t k) {
    seq_t x = 1, y = 0;
    while (k > 0) {
        if (k & 1) y += x; else x += y;
        k >>= 1;
    }
    return y;
}

seq_t a(idx_t n) {
    idx_t m = n + 1;
    seq_t s = 0;
    for (idx_t d = 1; d <= m / d; d++) {
        if (m % d == 0) {
            s += A071018_stern(d + 1);
            idx_t e = m / d;
            if (e != d) s += A071018_stern(e + 1);
        }
    }
    return s;
}
