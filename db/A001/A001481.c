/*
 * A001481  Numbers that are the sum of 2 squares.
 * https://oeis.org/A001481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy, core, changed
 * Formula:   search: a(n) is the n-th k >= 0 in which every prime p == 3 (mod
 *            4) divides k to an even power
 * Range:     u8 n <= 97, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9567 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static bool a001481_is_s2s(uint64_t m) {
    if (m == 0) return true;
    while ((m & 1) == 0) m >>= 1;
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            int cnt = 0;
            while (m % p == 0) {
                m /= p;
                cnt++;
            }
            if ((p & 3) == 3 && (cnt & 1)) return false;
        }
    }
    if (m > 1 && (m & 3) == 3) return false;
    return true;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t num = 0;
    for (;;) {
        if (a001481_is_s2s(num)) {
            count++;
            if (count == n) return (seq_t)num;
        }
        num++;
    }
}
