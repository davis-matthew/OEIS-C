/*
 * A228709  Numbers having in decimal representation at least one pair of
 *          consecutive digits with the same parity.
 * https://oeis.org/A228709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth positive integer (≥10) that has at least one pair
 *            * of consecutive digits of the same parity (both even or both
 *            odd). Computed * by a simple search
 * Range:     u8 n <= 163, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9542 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t, if needed */

static inline int has_pair(seq_t v) {
    int lastParity = -1;
    while (v > 0) {
        int d = (int)(v % 10);
        int p = d & 1;
        if (lastParity != -1 && p == lastParity)
            return 1;
        lastParity = p;
        v /= 10;
    }
    return 0;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;    /* offset is 1, but treat n==0 as 0 */

    seq_t x = 10;
    idx_t count = 0;
    while (1) {
        if (has_pair(x)) {
            ++count;
            if (count == n)
                return x;
        }
        ++x;
    }
}
