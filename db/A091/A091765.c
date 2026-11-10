/*
 * A091765  Numbers n such that A007306(n) divides n.
 * https://oeis.org/A091765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th m >= 0 (m = 0, 1, 2, ...) such that A007306(m)
 *            divides m; found by linear search. A007306(0) = 1 and A007306(m)
 *            = Stern's diatomic s(2m-1) for m > 0
 * Range:     u8 n <= 16, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A007306_stern(idx_t m) {
    seq_t x = 1, y = 0;
    while (m > 0) {
        if (m & 1) y += x;
        else x += y;
        m >>= 1;
    }
    return y;
}

static seq_t A007306_a(idx_t n) {
    if (n == 0) return 1;
    return A007306_stern(2 * n - 1);
}

seq_t a(idx_t n) {
    idx_t count = 0;
    idx_t m = 0;
    for (;; m++) {
        seq_t d = A007306_a(m);
        if (d == 0) continue;
        if (((seq_t)m % d) == 0) {
            count++;
            if (count == n) return (seq_t)m;
        }
    }
}
