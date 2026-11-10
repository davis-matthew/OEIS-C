/*
 * A071205  Largest digit of A071204(n).
 * https://oeis.org/A071205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A054055(A071204(n)); A071204(n) is the n-th number
 *            divisible by its largest decimal digit, found by a linear search
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Validated: partial, pass, 90 terms through n = 90 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned maxdigit_u64(uint64_t v) {
    unsigned md = 0;
    while (v > 0) {
        unsigned d = (unsigned)(v % 10);
        if (d > md) md = d;
        v /= 10;
    }
    return md;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t x = 0;
    unsigned d = 0;
    if (n == 0) return (seq_t)0;
    while (count < n) {
        x++;
        d = maxdigit_u64(x);
        if (d != 0 && x % d == 0) count++;
    }
    return (seq_t)d;
}
