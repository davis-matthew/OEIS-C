/*
 * A033738  Expansion of Product_{d | 26} theta_3(q^d).
 * https://oeis.org/A033738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions of a² + 2·b² + 13·c² + 26·d²
 *            = n (a,b,c,d ∈ ℤ)
 * Range:     u8 n <= 335, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root (floor) for unsigned 64‑bit values */
static inline uint64_t isqrt_u64(uint64_t x) {
    uint64_t lo = 0, hi = (uint64_t)1 << 32;            /* sqrt(2^64‑1) < 2^32 */
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        if (mid <= x / mid)  lo = mid;
        else                 hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n) {
    seq_t count = 0;

    uint64_t max_d = isqrt_u64(n / 26);
    for (uint64_t d = 0; d <= max_d; ++d) {
        uint64_t d2    = d * d;
        uint64_t dterm = 26 * d2;
        uint64_t fact_d = (d == 0) ? 1 : 2;
        uint64_t rem_d  = n - dterm;

        uint64_t max_c = isqrt_u64(rem_d / 13);
        for (uint64_t c = 0; c <= max_c; ++c) {
            uint64_t c2    = c * c;
            uint64_t cterm = 13 * c2;
            uint64_t fact_c = (c == 0) ? 1 : 2;
            uint64_t rem_dc  = rem_d - cterm;

            uint64_t max_b = isqrt_u64(rem_dc / 2);
            for (uint64_t b = 0; b <= max_b; ++b) {
                uint64_t b2    = b * b;
                uint64_t bterm = 2 * b2;
                uint64_t fact_b = (b == 0) ? 1 : 2;
                uint64_t rem    = rem_dc - bterm;

                uint64_t s = isqrt_u64(rem);
                if (s * s == rem) {
                    uint64_t fact_s = (s == 0) ? 1 : 2;
                    count += (seq_t)(fact_d * fact_c * fact_b * fact_s);
                }
            }
        }
    }

    return count;
}
