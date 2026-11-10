/*
 * A230088  Partial sums of A010062.
 * https://oeis.org/A230088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{k=0..n} b(k), where b(0)=1, b(k+1) = b(k) +
 *            popcount(b(k)) (A010062)
 * Range:     u8 n <= 14, u16 n <= 183, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 66 terms through n = 9959 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A010062_popcount(seq_t x) {
    seq_t c = 0;
    while (x) {
        c += x & 1;
        x >>= 1;
    }
    return c;
}

seq_t a(idx_t n) {
    seq_t b = 1;
    seq_t s = 1;
    for (idx_t i = 0; i < n; ++i) {
        b = b + A010062_popcount(b);
        s += b;
    }
    return s;
}
