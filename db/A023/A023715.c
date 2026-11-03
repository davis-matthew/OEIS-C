/*
 * A023715  Numbers with exactly 2 2's in their base 4 expansion.
 * https://oeis.org/A023715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th integer whose base-4 representation has exactly
 *            two digits equal to 2
 * Range:     u8 n <= 54, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9430 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static inline int count_two_base4(seq_t x) {
    int cnt = 0;
    while (x > 0) {
        if ((x & 3) == 2) cnt++;
        x >>= 2;
    }
    return cnt;
}

seq_t a(idx_t n) {
    seq_t val = 0;
    idx_t found = 0;
    for (;;) {
        if (count_two_base4(val) == 2) {
            found++;
            if (found == n) return val;
        }
        val++;
    }
}
