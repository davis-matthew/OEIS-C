/*
 * A210255  a(n) is the number of numbers m for which n is in interval
 *          (A007814(m!), A007814(m!) + A007814(m)].
 * https://oeis.org/A210255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of m ≥ 1 such that v2(m!) < n ≤ v2(m!) + v2(m),
 *            where v2(x) is the exponent of 2 in x
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static inline idx_t popcount_u64(idx_t x) {
    return (idx_t)__builtin_popcountll(x);
}

static inline idx_t tz_u64(idx_t x) {
    /* x is never 0 in this code */
    return (idx_t)__builtin_ctzll(x);
}

seq_t a(idx_t n) {
    seq_t count = 0;
    if (n == 0) return 0;          /* out of defined range, safe guard */

    for (idx_t m = 1;; ++m) {
        idx_t c = popcount_u64(m);      /* v2(m) = exponent of 2 in m */
        idx_t D = m - c;                /* v2(m!) = m - popcount(m) */

        if (D > n - 1) break;           /* no further m can satisfy */
        idx_t t = tz_u64(m);             /* trailing zeros of m */

        /* interval (D, D + t] */
        if (n > D && n <= D + t) ++count;
    }

    return count;
}
