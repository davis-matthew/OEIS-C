/*
 * A057030  Let P(n) of a sequence s(1),s(2),s(3),... be obtained by leaving
 *          s(1),...,s(n-1) fixed and reversing every n consecutive terms
 *          thereafter; apply P(2) to 1,2,3,... to get PS(2), then apply P(3)
 *          to PS(2) to get PS(3), then apply P(4) to PS(3), etc. The limit of
 *          PS(n) is A057030.
 * https://oeis.org/A057030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = A007062(n+1) - 1 for n >= 1, where A007062(N) is found by
 *            tracing index N backwards through the block reversals for m =
 *            N-1 down to 2
 * Range:     u8 n <= 31, u16 n <= 534, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 67 terms through n = 4963 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A007062_a(idx_t n) {
    uint64_t p = (uint64_t)n;
    if (n <= 2) return (seq_t)p;
    for (uint64_t m = (uint64_t)n - 1; m >= 2; m--) {
        if (p > m) {
            uint64_t q = p - m - 1;
            uint64_t k = q / m;
            uint64_t r = q % m;
            p = m + 1 + k * m + (m - 1 - r);
        }
    }
    return (seq_t)p;
}

seq_t a(idx_t n) {
    return A007062_a(n + 1) - 1;
}
