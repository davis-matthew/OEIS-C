/*
 * A064833  Indices of record numbers in A064791.
 * https://oeis.org/A064833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index of the n-th record (strict left-to-right maximum)
 *            in A064791; found by scanning m = 1, 2, ... and tracking the
 *            running maximum of A064791(m)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Validated: partial, pass, 13 terms through n = 13 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A064791_a(idx_t n) {
    seq_t b = (seq_t)n;
    for (seq_t k = 1; ; ++k) {
        if (k > b) break;
        if (b >= k + 1 && b <= 2 * k) {
            b += k;
        } else if (b >= 2 * k + 1 && b <= 3 * k) {
            b -= k;
        }
    }
    return b;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t best = 0;
    for (idx_t m = 1; ; ++m) {
        seq_t v = A064791_a(m);
        if (v > best) {
            best = v;
            ++count;
            if (count == n) return (seq_t)m;
        }
    }
}
