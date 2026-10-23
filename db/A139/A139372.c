/*
 * A139372  Let the binary expansion of n be n = Sum_{k} 2^{r_k}, let e(n) be
 *          the number of r_k's that are even, o(n) the number that are odd;
 *          sequence lists n such that e(n) >= o(n).
 * https://oeis.org/A139372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 163, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A139372: numbers n such that e(n) >= o(n) where
 * e(n) counts set bits at even positions and o(n) counts set bits
 * at odd positions.  Computation is performed by brute‑force searching
 * the integers in increasing order. */
#include <stdint.h>
#include <stdbool.h>

static bool good_value(idx_t m) {
    idx_t x = m;
    idx_t e = 0, o = 0;
    idx_t pos = 0;
    while (x) {
        if (x & 1ULL) {
            if ((pos & 1ULL) == 0) ++e; else ++o;
        }
        x >>= 1;
        ++pos;
    }
    return e >= o;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* not used, but harmless */
    idx_t count = 0;
    for (idx_t m = 0; ; ++m) {
        if (good_value(m)) {
            ++count;
            if (count == n) return (seq_t)m;
        }
    }
}
