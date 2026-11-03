/*
 * A000414  Numbers that are the sum of 4 nonzero squares.
 * https://oeis.org/A000414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th positive integer that is not in A000534. The
 *            exceptions are 1,3,5,9,11,17,29,41 and 2*4^k, 6*4^k, 14*4^k for
 *            k >= 0. The code finds the smallest m with m - #{exceptions <=
 *            m} = n
 * Range:     u8 n <= 237, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9664 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t exc_count(uint64_t m) {
    static const uint64_t fixed[8] = {1, 3, 5, 9, 11, 17, 29, 41};
    static const uint64_t bases[3] = {2, 6, 14};
    uint64_t c = 0;
    int i;
    for (i = 0; i < 8; i++)
        if (fixed[i] <= m) c++;
    for (i = 0; i < 3; i++) {
        uint64_t v = bases[i];
        while (v <= m) {
            c++;
            if (v > m / 4) break;
            v *= 4;
        }
    }
    return c;
}

seq_t a(idx_t n) {
    uint64_t m = (uint64_t)n;
    for (;;) {
        uint64_t f = m - exc_count(m);
        if (f >= (uint64_t)n) break;
        m += (uint64_t)n - f;
    }
    return (seq_t)m;
}
