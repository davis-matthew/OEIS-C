/*
 * A074644  a(n) = (A074639(n^2) - A074639(n)) mod n.
 * https://oeis.org/A074644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A074639(n^2) - A074639(n)) mod n, where A074639(m) =
 *            Sum_{h in [1,m], (h,m)=1} (h*h' - 1)/m with h*h' == 1 (mod m);
 *            computed by direct summation modulo n
 * Range:     u8 n <= 383, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Validated: standard, pass, 118 terms through n = 2429 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A074644_sum_mod(uint64_t m, uint64_t n)
{
    uint64_t sum = 0;
    uint64_t h;
    if (m < 4294967296ULL) {
        for (h = 1; h < m; ++h) {
            uint32_t r0 = (uint32_t)h, r1 = (uint32_t)m;
            int64_t s0 = 1, s1 = 0;
            while (r0 != 0) {
                uint32_t q = r1 / r0;
                uint32_t r2 = r1 - q * r0;
                r1 = r0; r0 = r2;
                int64_t s2 = s1 - (int64_t)q * s0;
                s1 = s0; s0 = s2;
            }
            /* r1 is gcd, s1 is coefficient of h */
            if (r1 != 1) continue;
            int64_t inv = s1 % (int64_t)m;
            if (inv < 0) inv += (int64_t)m;
            uint64_t k = (h * (uint64_t)inv - 1) / m;
            sum += k % n;
            if (sum >= n) sum -= n;
        }
    } else {
        for (h = 1; h < m; ++h) {
            uint64_t r0 = h, r1 = m;
            int64_t s0 = 1, s1 = 0;
            while (r0 != 0) {
                uint64_t q = r1 / r0;
                uint64_t r2 = r1 - q * r0;
                r1 = r0; r0 = r2;
                int64_t s2 = s1 - (int64_t)q * s0;
                s1 = s0; s0 = s2;
            }
            if (r1 != 1) continue;
            int64_t inv = s1 % (int64_t)m;
            if (inv < 0) inv += (int64_t)m;
            uint64_t k = (h * (uint64_t)inv - 1) / m;
            sum += k % n;
            if (sum >= n) sum -= n;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;
    uint64_t nn = (uint64_t)n;
    uint64_t s2 = A074644_sum_mod(nn * nn, nn);
    uint64_t s1 = A074644_sum_mod(nn, nn);
    uint64_t r = (s2 + nn - s1) % nn;
    return (seq_t)r;
}
