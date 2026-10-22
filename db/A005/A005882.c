/*
 * A005882  Theta series of planar hexagonal lattice (A2) with respect to deep
 *          hole.
 * https://oeis.org/A005882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 * Sum_{d | 3n+1} chi(d), where chi(d) = 1 if d == 1
 *            (mod 3), -1 if d == 2 (mod 3), 0 otherwise (a(n) = 3*A033687(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int chi3(uint64_t d) {
    uint64_t r = d % 3;
    if (r == 1) return 1;
    if (r == 2) return -1;
    return 0;
}

seq_t a(idx_t n) {
    uint64_t m = 3 * (uint64_t)n + 1;
    long long s = 0;
    uint64_t d;
    for (d = 1; d * d <= m; d++) {
        if (m % d == 0) {
            uint64_t e = m / d;
            s += chi3(d);
            if (e != d) s += chi3(e);
        }
    }
    if (s < 0) s = 0;
    return (seq_t)3 * (seq_t)(uint64_t)s;
}
