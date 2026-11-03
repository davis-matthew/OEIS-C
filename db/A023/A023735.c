/*
 * A023735  Numbers with exactly 2 3's in their base-5 expansion.
 * https://oeis.org/A023735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 68 terms through n = 9349 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A023735: Numbers with exactly 2 3's in base‑5 expansion */
#include <stdbool.h>

static bool two_threes_in_base5(seq_t x) {
    idx_t cnt = 0;
    while (x) {
        if (x % 5 == 3) {
            ++cnt;
            if (cnt > 2) return false;
        }
        x /= 5;
    }
    return cnt == 2;
}

seq_t a(idx_t n) {
    seq_t val = 0;
    idx_t found = 0;
    while (found < n) {
        if (two_threes_in_base5(val)) {
            ++found;
            if (found == n) {
                return val;
            }
        }
        ++val;
    }
    return val; /* unreachable */
}
