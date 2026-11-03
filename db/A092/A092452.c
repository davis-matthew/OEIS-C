/*
 * A092452  Numbers that either contain the digit 3 or are divisible by 3.
 * https://oeis.org/A092452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 120, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Validated: standard, pass, 82 terms through n = 1983 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static inline bool contains3(seq_t v) {
    if (v == 0) return false;
    seq_t t = v;
    while (t != 0) {
        if ((t % 10) == 3) return true;
        t /= 10;
    }
    return false;
}

/* Sequence: Numbers that either contain the digit 3 or are divisible by 3.
   Computed by a simple linear scan. */

seq_t a(idx_t n) {
    seq_t cur = 0;
    idx_t count = 0;
    for (;;) {
        if ((cur % 3 == (seq_t)0) || contains3(cur)) {
            if (count == n) return cur;
            ++count;
        }
        ++cur;
    }
}
